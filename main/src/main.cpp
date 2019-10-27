#include <iostream>
#include <string>

#include "game_object.hpp"

int main(int argc, char* argv[])
{
	bool choice;
	std::string ip;
	std::string port;

	ip = argv[2];
	port = argv[3];


	if (argv[1] == "server") 
	{
		std::string server;
		server = argv[1];

		//create server

		GameObject object1;
		GameObject object2;
		GameObject object3;

		std::cin.ignore();

		//Enregistrer dans gestionnaire de réplication
		//Supprimer l'objet

	}
	else if(argv[1] == "server")
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