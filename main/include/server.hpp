#pragma once

#include <uvw.hpp>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>

namespace server
{
    class Server
    {
        public:

            Server(const std::string& ip, uint16_t port) noexcept;
            virtual ~Server() noexcept;
            Server(Server&& other) = default;
            Server(const Server& other) = delete;
            Server& operator=(const Server& s) = delete;
            Server& operator=(Server&& s) = default;
            void Send(uint8_t* ptr, int SIZE) noexcept;
            
        private:

       		std::vector<std::shared_ptr<uvw::TCPHandle>> m_listClient;
            std::unique_ptr<std::thread> m_loopThread;
            std::shared_ptr<uvw::Loop> m_loop;
			std::shared_ptr<uvw::TCPHandle> m_socket;
    };

}
