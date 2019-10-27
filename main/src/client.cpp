#include "client.hpp"

namespace client
{
    Client::Client(const std::string& ip, uint16_t port) noexcept :
        m_ConnectionClient(), m_repManager(), m_loop(uvw::Loop::getDefault())
        {
            auto tcp = m_loop->resource<uvw::TCPHandle>();
            m_loop->run();

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
                gsl::span<char> receivedData(evt.data.get(), evt.length);
                InputStream receivedStream(receivedData);
                m_repManager.replicate(receivedStream);
                std::vector<GameObject> vectObjects = m_repManager.getObjects();
                std::for_each(vectObjects.begin(), vectObjects.end(), [this](GameObject Obj) 
                    {
                        std::cout << "Replication data : " << Obj.ClassID() << " received." << std::endl;
                    }
                );
            });

            m_ConnectionClient = tcp;

            tcp->connect(ip, port);
        }

        Client::~Client() noexcept
        {
            m_ConnectionClient->stop();
            m_ConnectionClient->close();
        }
}