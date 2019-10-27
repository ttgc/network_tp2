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
#include "coordinates.hpp"

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



		
		GameObject object;

		vect.push_back(&object);
		replication.lock()->replicate(stream, vect);

		std::cout << "Object has been added." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());
		
		std::cin.ignore();

		Player player;

		coordinates::Position pos = { 50.0, 450.5, 10.333 };
		coordinates::Quaternion rot = { -0.4, 0.4, 0.005, -0.455 };

		player.setName("Jean michel");
		player.setPosition(pos);
		player.setRotation(rot);

		vect.push_back(&player);

		replication.lock()->replicate(stream, vect);

		std::cout << "Player has been added." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();



		Enemy enemy;

		pos = { 500.0, 12.5, 450.333 };
		rot = { 0.4, -0.356, -0.5, -0.875 };

		enemy.setType("Goblin");
		enemy.setPosition(pos);
		enemy.setRotation(rot);

		vect.push_back(&enemy);
		replication.lock()->replicate(stream, vect);

		std::cout << "Enemy has been added." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


		pos = { 12.05, 16.25, 254.333 };
		rot = { 0.054, 0.536, -0.005, -0.875 };

		player.setName("New name");
		player.setPosition(pos);
		player.setRotation(rot);

		std::cout << "Name, position and rotation of player updated." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();

		pos = { 26, 334.632, 159.053 };
		rot = { 0.054, 0.536, -0.005, -0.875 };

		enemy.setType("Troll");
		enemy.setPosition(pos);
		enemy.setRotation(rot);

		std::cout << "Type, position and rotation of enemy updated." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


		vect.erase(vect.begin());

		std::cout << "Object has been deleted." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


	}
	else if(argv[1] == "client")
	{
		
		client::Client client(ip, port);

	}
	else 
	{
		std::cout << "Please choose wisely between server or client in the command line" << std::endl;
	}
	
}