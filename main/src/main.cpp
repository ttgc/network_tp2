#include <iostream>
#include <string>
#include <vector>


#include "game_object.hpp"
#include "classregistry.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "server.hpp"
#include "client.hpp"
#include "replicationmanager.hpp"
#include "streams.hpp"

int main(int argc, char* argv[])
{
	bool choice;
	std::string ip;
	uint16_t port;
	
	ip = argv[2];
	port = atoi(argv[3]);

	

	if (argc < 3) 
	{
		return EXIT_FAILURE;
	}

	if (argv[1] == "server") 
	{
		
		server::Server server(ip, port);
		const auto registry = ClassRegistry::get();
		const auto replication = ReplicationManager::get();
		
		OutputStream stream;

		std::vector<GameObject*> vect;

		registry.lock()->registerClass<GameObject>(GameObject::CreateInstance);
		registry.lock()->registerClass<Player>(Player::CreateInstance);
		registry.lock()->registerClass<Enemy>(Enemy::CreateInstance);


		//create server

		
		GameObject object;

		vect.push_back(&object);
		replication.lock()->replicate(stream, vect);
		
		std::cin.ignore();

		Player player;

		vect.push_back(&player);
		replication.lock()->replicate(stream, vect);

		std::cin.ignore();

		Enemy enemy;

		vect.push_back(&enemy);
		replication.lock()->replicate(stream, vect);

		std::cin.ignore();

		vect.erase(vect.begin());

		std::cin.ignore();

		//Supprimer l'objet

	}
	else if(argv[1] == "client")
	{
		std::string client;
		client = argv[1];

		//Create and start a client

		//Receive data from replication

		//print objects
	}
	else 
	{
		std::cout << "Please choose wisely between server or client in the command line" << std::endl;
	}
	
}