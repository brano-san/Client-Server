#include <iostream>
#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	std::cout << "Server side" << '\n' << '\n';
	try
	{
		std::string ip("127.0.0.1");
		constexpr unsigned int port = 3500;
		auto server = my_chat::Server(std::move(ip), port);
		server.openConnection();
		server.initClient();

		server.sendToClient("Hello, Client!!!");

		std::cout << server.receiveFromClient() << '\n';
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << '\n';
	}

	return EXIT_SUCCESS;
}
