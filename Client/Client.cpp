#include "Client.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <WS2tcpip.h>

Client::Client(std::string ip, const unsigned short port)
	: _serverIp(std::move(ip)) ,_serverPort(port)
{
	ZeroMemory(&(this->_wsaData),    sizeof(WSADATA));
	ZeroMemory(&(this->_socket),     sizeof(SOCKET));
	ZeroMemory(&(this->_serverInfo), sizeof(sockaddr_in));

	this->winSockInit();
}

Client::~Client()
{
	closesocket(this->_socket);
	WSACleanup();
}

void Client::connectToServer()
{
	const auto serverInfo = reinterpret_cast<sockaddr*>(&this->_serverInfo);
	if (connect(this->_socket, serverInfo, sizeof(sockaddr_in)) != 0)
	{
		throw std::runtime_error("Cannot connect to server: " + std::to_string(WSAGetLastError()));
	}

	std::vector<char> message(50);
	std::fgets(message.data(), message.size(), stdin);

	send(this->_socket, message.data(), message.size(), 0);
}

void Client::winSockInit()
{
	if (WSAStartup(MAKEWORD(2,2), &this->_wsaData) != 0)
	{
		throw std::runtime_error("Cannot initialize WinSock: " + std::to_string(WSAGetLastError()));
	}

	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Cannot initialize Socket: " + std::to_string(WSAGetLastError()));
	}

	in_addr serverIp;
	if (inet_pton(AF_INET, _serverIp.c_str(), &serverIp) <= 0)
	{
		throw std::runtime_error("Cannot cast ip: " + std::to_string(WSAGetLastError()));
	}

	this->_serverInfo.sin_family = AF_INET;
	this->_serverInfo.sin_port = htons(this->_serverPort);
	this->_serverInfo.sin_addr = serverIp;
}
