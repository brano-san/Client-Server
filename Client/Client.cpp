#include "Client.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <WS2tcpip.h>

my_chat::Client::Client()
	: Client(std::string(), 0)
{
	
}

my_chat::Client::Client(std::string ip, const unsigned short port)
	: _serverIp(std::move(ip)) ,_serverPort(port)
{
	ZeroMemory(&(this->_wsaData),      sizeof(WSADATA));
	ZeroMemory(&(this->_serverSocket), sizeof(SOCKET));
	ZeroMemory(&(this->_serverInfo),   sizeof(sockaddr_in));

	if (WSAStartup(MAKEWORD(2, 2), &this->_wsaData) != 0)
	{
		throw std::runtime_error("Cannot initialize WinSock: " +
			std::to_string(WSAGetLastError()));
	}
}

my_chat::Client::~Client()
{
	closesocket(this->_serverSocket);
	WSACleanup();
}

void my_chat::Client::openConnection()
{
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_serverSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("Cannot initialize Server Socket: " +
			std::to_string(WSAGetLastError()));
	}

	in_addr serverIp;
	if (inet_pton(AF_INET, _serverIp.c_str(), &serverIp) <= 0)
	{
		throw std::runtime_error("Cannot cast ip: " +
			std::to_string(WSAGetLastError()));
	}

	this->_serverInfo.sin_family = AF_INET;
	this->_serverInfo.sin_port = htons(this->_serverPort);
	this->_serverInfo.sin_addr = serverIp;

	const auto serverInfo = reinterpret_cast<sockaddr*>(&this->_serverInfo);
	if (connect(this->_serverSocket, serverInfo, sizeof(sockaddr_in)) != 0)
	{
		throw std::runtime_error("Cannot connect to server: " +
			std::to_string(WSAGetLastError()));
	}
}

std::string my_chat::Client::receiveFromServer()
{
	std::vector<char> message(50);

	recv(this->_serverSocket, message.data(), message.size(), 0);

	return std::string{ message.begin(), message.end() };
}

void my_chat::Client::sendToServer(const std::string& message)
{
	send(this->_serverSocket, message.data(), message.size(), 0);
}

void my_chat::Client::setIpAndPort(const std::string& ip, const unsigned short port)
{
	this->_serverIp = ip;
	this->_serverPort = port;
}
