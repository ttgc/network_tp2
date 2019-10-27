#include "client.hpp"
#include "player.hpp"
#include "enemy.hpp"

namespace client
{
    Client::Client(const std::string& ip, uint16_t port) noexcept :
        m_ConnectionClient(), m_loop(uvw::Loop::getDefault())
        {
            auto tcp = m_loop->resource<uvw::TCPHandle>();

            tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &)
            {
                std::cout << "An error as occured" << std::endl; 
            });

            tcp->once<uvw::ConnectEvent>([](const uvw::ConnectEvent &, uvw::TCPHandle &tcp) 
            {
                std::cout << "Connected to Server" << std::endl;
                tcp.close();
            });

            tcp->on<uvw::DataEvent>([this](const uvw::DataEvent& evt, uvw::TCPHandle &)
            {
                const auto repManager = ReplicationManager::get();
                gsl::span<char> receivedData(evt.data.get(), evt.length);
                InputStream receivedStream(receivedData);
                repManager.lock()->replicate(receivedStream);
                std::vector<GameObject> vectObjects = repManager.lock()->getObjects();
                std::for_each(vectObjects.begin(), vectObjects.end(), [this](GameObject Obj) 
                    {
                        std::cout << "Replication data : " << Obj.ClassID() << " received." << std::endl;
						if (Obj.ClassID() == 1)
						{
							const auto player = reinterpret_cast<Player*>(&Obj);
							const auto pos = player->getPosition();
							const auto rot = player->getRotation();
							std::cout << "Player " << player->getName() << ": (" << 
								pos.x << ", " << pos.y << ", " << pos.z << ") - (" <<
								rot.x << ", " << rot.y << ", " << rot.z << ", " << rot.w << ")" << 
								std::endl;
						}
						else if (Obj.ClassID() == 2)
						{
							const auto enemy = reinterpret_cast<Enemy*>(&Obj);
							const auto pos = enemy->getPosition();
							const auto rot = enemy->getRotation();
							std::cout << "Enemy " << enemy->getType() << ": (" <<
								pos.x << ", " << pos.y << ", " << pos.z << ") - (" <<
								rot.x << ", " << rot.y << ", " << rot.z << ", " << rot.w << ")" <<
								std::endl;
						}
                    }
                );
            });

            m_ConnectionClient = tcp;

            tcp->connect(ip, port);
            
            m_loop->run();
        }

        Client::~Client() noexcept
        {
            m_ConnectionClient->stop();
            m_ConnectionClient->clear();
            m_ConnectionClient->close();
        }
}