#include <iostream>
#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try
	{
		const std::string ip("127.0.0.1");
		const unsigned int port = 3500;
		auto client = Client(std::move(ip), port);
		client.connectToServer();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << '\n';
	}

	return EXIT_SUCCESS;
}
