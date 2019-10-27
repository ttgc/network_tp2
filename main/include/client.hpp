#pragma once

#include <uvw.hpp>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <stream.hpp>
#include "replicationmanager.hpp"

namespace client
{
    class Client
    {
        public:

            Client(const std::string& ip, uint16_t port) noexcept;
            virtual ~Client() noexcept;
            Client(Client&& other) = default;
            Client(const Client& other) = default;
            Client& operator=(const Client& s) = delete;

        private:

       		std::shared_ptr<uvw::TCPHandle> m_ConnectionClient;
            ReplicationManager m_repManager;
    };

}