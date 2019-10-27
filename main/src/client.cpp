#include "client.hpp"

namespace client
{
    Client::Client(const std::string& ip, uint16_t port) noexcept :
        m_ConnectionClient(), m_repManager()
        {
            auto loop = uvw::Loop::getDefault(); 
            // mettre la loop en paramètre et ne pas l'initialiser dans le constructeur ?
            auto tcp = loop->resource<uvw::TCPHandle>();

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
/*
mkdir output
cd output/
cmake ..
cmake --build .
*/
        Client::~Client() noexcept
        {
            m_ConnectionClient->stop();
            m_ConnectionClient->close();
        }
}