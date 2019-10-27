#include <iostream>
#include <string>

#include "game_object.hpp"
#include "classregistry.hpp"
#include "enemy.hpp"
#include "player.hpp"

int main(int argc, char* argv[])
{
	bool choice;
	std::string ip;
	std::string port;

	ip = argv[2];
	port = argv[3];


	if (argv[1] == "server") 
	{

		const auto registry = ClassRegistry::get();

		std::string server;
		server = argv[1];

		//create server

		registry.lock()->registerClass<GameObject>(GameObject::CreateInstance);
		std::cin.ignore();

		registry.lock()->registerClass<Player>(Player::CreateInstance);
		std::cin.ignore();

		registry.lock()->registerClass<Enemy>(Enemy::CreateInstance);

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