#include "client.hpp"

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