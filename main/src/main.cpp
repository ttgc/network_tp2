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
	if (argc < 4) 
	{
		return EXIT_FAILURE;
	}

	std::string mode(argv[1]);
	std::string ip(argv[2]);
	uint16_t port(atoi(argv[3]));


	const auto registry = ClassRegistry::get();
	registry.lock()->registerClass<GameObject>(GameObject::CreateInstance);
	registry.lock()->registerClass<Player>(Player::CreateInstance);
	registry.lock()->registerClass<Enemy>(Enemy::CreateInstance);

	if (mode == "server") 
	{
		
		server::Server server(ip, port);
		std::cout << "server is running on " << ip << ":" << port << std::endl;
		std::cin.ignore();

		const auto replication = ReplicationManager::get();
		
		OutputStream stream;

		std::vector<GameObject*> vect;
		
		GameObject object;

		vect.push_back(&object);
		replication.lock()->replicate(stream, vect);

		std::cout << "Object has been added." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());
		
		std::cin.ignore();

		Player player;

		coordinates::Position pos = { 50.0, 450.5, 10.333 };
		coordinates::Quaternion rot = { -0.763, -0.002, -0.614, 0.205 };

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
		rot = { -0.607, 0.002, 0.754, -0.251 };

		enemy.setType("Goblin");
		enemy.setPosition(pos);
		enemy.setRotation(rot);

		vect.push_back(&enemy);
		replication.lock()->replicate(stream, vect);

		std::cout << "Enemy has been added." << std::endl;

		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


		pos = { 12.05, 16.25, 254.333 };
		rot = { -0.607, 0.002, 0.754, -0.251 };

		reinterpret_cast<Player*>(vect[1])->setName("New name");
		reinterpret_cast<Player*>(vect[1])->setPosition(pos);
		reinterpret_cast<Player*>(vect[1])->setRotation(rot);

		std::cout << "Name, position and rotation of player updated." << std::endl;

		replication.lock()->replicate(stream, vect);
		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();

		pos = { 26, 334.632, 159.053 };
		rot = { -0.763, -0.002, -0.614, 0.205 };

		reinterpret_cast<Enemy*>(vect[2])->setType("Troll");
		reinterpret_cast<Enemy*>(vect[2])->setPosition(pos);
		reinterpret_cast<Enemy*>(vect[2])->setRotation(rot);

		std::cout << "Type, position and rotation of enemy updated." << std::endl;

		replication.lock()->replicate(stream, vect);
		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


		vect.erase(vect.begin());

		std::cout << "Object has been deleted." << std::endl;

		replication.lock()->replicate(stream, vect);
		server.Send(reinterpret_cast<uint8_t*>(stream.Data().data()), stream.Size());

		std::cin.ignore();


	}
	else if(mode == "client")
	{
		
		client::Client client(ip, port);
	}
	else 
	{
		std::cout << "Please choose wisely between server or client in the command line" << std::endl;
	}
	
}