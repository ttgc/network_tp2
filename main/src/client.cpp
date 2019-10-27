#include "client.hpp"

namespace client
{
    Client::Client(const std::string& ip, uint16_t port) noexcept :
        m_ConnectionClient()//, m_repManager()
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

            tcp->on<uvw::DataEvent>([](const uvw::DataEvent& evt, uvw::TCPHandle &)
            {
                std::cout << evt.data << std::endl;
                gsl::span<char> receivedData(evt.data.get(), evt.length);
                InputStream receivedStream(receivedData);
                m_repManager.Replicate(receivedStream);
            });

            m_ConnectionClient = tcp;

            tcp->connect(ip, port);
        }

        Client::~Client() noexcept
        {
            m_ConnectionClient->close();
        }
}