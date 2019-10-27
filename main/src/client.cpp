#include "client.hpp"

namespace client
{
    Client::Client(const std::string& ip, uint16_t port) noexcept :
        m_runThread(nullptr), m_ConnectionClient()
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
                //auto dataWrite = std::unique_ptr<char[]>(new char[2]{ 'b', 'c' });
                //tcp.write(std::move(dataWrite), 2);
                //Pas besoin de write de data, le client n'envoie rien au serveur.

                tcp.close();
            });

            tcp->on<uvw::DataEvent>([](const uvw::DataEvent& evt, uvw::TCPHandle &)
            {
                //reçoit les données du send dans evt, doit déserialiser
                std::cout << "Afficher le type d'objet reçu, voir avec Thomas pour sa replication" << std::endl;
            });

            m_ConnectionClient = tcp;

            tcp->connect(ip, port);

            //m_runThread = std::make_unique<std::thread>([this]() { run(); });
            // Grâce à uvw, pas besoin de Thread ?
        }

        Client::~Client() noexcept
        {
            m_ConnectionClient->close();
        }

/*        void run()
        {

        }
*/
}