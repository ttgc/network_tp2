#include "server.hpp"

namespace server
{
    Server::Server(const std::string& ip, uint16_t port) noexcept :
        m_listClient()
        {
            auto loop = uvw::Loop::getDefault();
            std::shared_ptr<uvw::TCPHandle> tcp = loop->resource<uvw::TCPHandle>();

            tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) 
            {
                std::cout << "An error as occured" << std::endl;
            });

            tcp->on<uvw::ListenEvent>([this](const uvw::ListenEvent &, uvw::TCPHandle &srv) {
                std::shared_ptr<uvw::TCPHandle> client = srv.loop().resource<uvw::TCPHandle>();

                client->on<uvw::CloseEvent>([ptr = srv.shared_from_this(), this](const uvw::CloseEvent &, uvw::TCPHandle &) 
                {
                    ptr->close();
                    if (!m_listClient.empty())
                    {
                        auto res = std::remove(m_listClient.begin(), m_listClient.end(), ptr);
                        m_listClient.erase(res, m_listClient.end());
                    }
                });
                
                client->on<uvw::EndEvent>([this](const uvw::EndEvent &, uvw::TCPHandle &client) 
                {
                    client.close();
                    if (!m_listClient.empty())
                    {
                        auto res = std::remove(m_listClient.begin(), m_listClient.end(), client);
                        m_listClient.erase(res, m_listClient.end());
                    }
                });
                srv.accept(*client);
                client->read();
                m_listClient.push_back(client);
            });
        
            tcp->bind(ip, port);
            tcp->listen();
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
        }

    void Server::Send(uint8_t* ptr, int SIZE) noexcept
    {
        char* pointeur = reinterpret_cast<char*>(ptr);
        if(!m_listClient.empty())
        {
            std::for_each(m_listClient.begin(), m_listClient.end(),[pointeur, SIZE](std::shared_ptr<uvw::TCPHandle> client) {
                client->write(pointeur, SIZE);
            });

        }
    }
}
