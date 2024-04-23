#include <iostream>
#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try
	{
		std::string ip("127.0.0.1");
		const unsigned int port = 3500;
		auto client = Server(std::move(ip), port);
		client.listenToClient();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << '\n';
	}

	return EXIT_SUCCESS;
}
