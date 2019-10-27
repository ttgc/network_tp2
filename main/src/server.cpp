#include "server.hpp"

namespace server
{
    Server::Server(const std::string& ip, uint16_t port) noexcept :
        m_listClient(), m_loopThread(nullptr), m_loop(uvw::Loop::getDefault())
        {   
            std::shared_ptr<uvw::TCPHandle> tcp = m_loop->resource<uvw::TCPHandle>();

            tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) 
            {
                std::cout << "An error as occured" << std::endl;
            });

            tcp->on<uvw::ListenEvent>([this](const uvw::ListenEvent &, uvw::TCPHandle &srv) {
                std::shared_ptr<uvw::TCPHandle> client = srv.loop().resource<uvw::TCPHandle>();

                client->on<uvw::CloseEvent>([ptr = srv.shared_from_this(), this](const uvw::CloseEvent &, uvw::TCPHandle &) 
                {
                    ptr->close();
                    auto res = std::remove_if(m_listClient.begin(), m_listClient.end(), [](std::shared_ptr<uvw::TCPHandle> cli) { 
                        return (cli == nullptr || cli->closing());
                    });
                });
                
                client->on<uvw::EndEvent>([this](const uvw::EndEvent &, uvw::TCPHandle &client) 
                {
                    client.close();
                    auto res = std::remove_if(m_listClient.begin(), m_listClient.end(), [](std::shared_ptr<uvw::TCPHandle> cli) { 
                        return (cli == nullptr || cli->closing());
                    });
                });
                srv.accept(*client);
                client->read();
                m_listClient.push_back(client);
            });
        
            tcp->bind(ip, port);
            tcp->listen();

            m_loopThread = std::make_unique<std::thread>([this]() { m_loop->run(); });
        }

        Server::~Server() noexcept
        {
            std::for_each(m_listClient.begin(), m_listClient.end(),
                [this](std::shared_ptr<uvw::TCPHandle> client) {
                    client->stop();
                    client->clear();
                    client->close();
                }
            );
            m_listClient.clear();

			if (m_loopThread != nullptr && m_loopThread->joinable())
			{
				m_loopThread->join();
			}
        }

    void Server::Send(uint8_t* ptr, int SIZE) noexcept
    {
        char* pointeur = reinterpret_cast<char*>(ptr);
        std::for_each(m_listClient.begin(), m_listClient.end(),[pointeur, SIZE](std::shared_ptr<uvw::TCPHandle> client) {
            if ((client != nullptr) && (client->active()))
            client->write(pointeur, SIZE);
        });
    
    }
}
