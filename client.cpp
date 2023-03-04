#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include "base.h"

#define DEFAULT_PORT 23410
#define MAX_LENGTH 1024


int main(int argc, char* argv[])
{
	char* serverIp = argv[1];

	hostent* host = gethostbyname(serverIp);
	sockaddr_in sendSockAddr;
	bzero(&sendSockAddr, sizeof(sendSockAddr));//инициализация
	sendSockAddr.sin_family = AF_INET;
	sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)*host->h_addr_list));
	sendSockAddr.sin_port = htons(DEFAULT_PORT);
	int clientSd = Socket(AF_INET, SOCK_STREAM, 0);

	//подключение
	Connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
	std::cout << "Connected to the server." << std::endl;

	int bytesRead, bytesWritten = 0;
	char msg[DEFAULT_PORT];

	while(1)
	{
		std::cout << ">";
		std::string data;
		std::getline(std::cin, data);
		memset(&msg, 0, sizeof(msg));//очистка буфера
		strcpy(msg, data.c_str());
		if(data == "exit")
		{
			send(clientSd, &msg, sizeof(msg), 0);
			break;
		}
		bytesWritten += send(clientSd, &msg, sizeof(msg), 0);
		std::cout << "Awaiting server response..." << std::endl;
		memset(&msg, 0, sizeof(msg));//очистка буфера
		bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
		if(!strcmp(msg, "exit"))
		{
			std::cout << "Server has quit the session" << std::endl;
			break;
		}
		std::cout << "SERVER: " << msg << std::endl;
	}
	close(clientSd);
	std::cout << "Connection closed" << std::endl;
	std::cout << "Byttes written: " << bytesWritten << std::endl;
	std::cout << "Byttes readen: " << bytesRead << std::endl;
	return 0;
}