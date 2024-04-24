#include <iostream>
#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	std::cout << "Client side" << '\n' << '\n';
	try
	{
		std::string ip("127.0.0.1");
		const unsigned int port = 3500;

		auto client = my_chat::Client(std::move(ip), port);
		client.openConnection();

		std::cout << client.receiveFromServer() << '\n';

		client.sendToServer("Hello, Brother!!!");
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << '\n';
	}

	return EXIT_SUCCESS;
}
