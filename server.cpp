#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <cstring>
#include "base.h"


#define DEFAULT_PORT 23410
#define MAX_LENGTH 1024

int main(int argc, char* argv[])
{
	//настраиваем сокет
	sockaddr_in servAddr;
	bzero(&servAddr, sizeof(servAddr));//инициализируем структуру
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//segmentation fault
	servAddr.sin_port = htons(DEFAULT_PORT);

	//создаем сокет
	int serverSd = Socket(AF_INET, SOCK_STREAM, 0);

	//связываем сокет с локальным адресом
	Bind(serverSd, (sockaddr*) &servAddr, sizeof(servAddr));

	std::cout << "Waiting for a client to connect..." << std::endl;

	//прослушиваем до 5 запросов одновременно
	listen(serverSd, 5);
	//то есть мы прослушиваем на каком-то определенном сокете

	//нужен новый адррес для коннекта с клиентом
	sockaddr_in newSockAddr;
	socklen_t newSockAddrSize = sizeof(newSockAddr);

	//принимаем на сокете сервера serverSd
	int newSd = Accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);//размер адреса передается в accept по ссылке!

	std::cout << "Connected with client." << std::endl;

	char msg[MAX_LENGTH];
	int bytesRead, bytesWritten = 0;

	while(1)
	{
		std::cout << "Awaiting client response..." << std::endl;
		memset(&msg, 0, sizeof(msg));//очистка буфера

		//принимаем сообщение от клиаента с newSd и записываем в msg
		bytesRead += recv(newSd, &msg, sizeof(msg), 0);

		if(!strcmp(msg, "exit"))
		{
			std::cout << "Client has quit the session" << std::endl;
			break;
		}

		std::cout << "CLIENT: " << msg << std::endl;
		std::cout << ">";
		std::string data;//сообщение, которое отпраялется клиенту
		getline(std::cin, data);
		memset(&msg, 0, sizeof(msg));//очистка буфера
		strcpy(msg, data.c_str());//c_str() возвращает массив чаров от строки

		if(data == "exit")
		{
			send(newSd, &msg, sizeof(msg), 0);//отдельно, чтобы не считать как bytesWritten
			break;
		}

		//отправляем сооьщение клиенту
		bytesWritten += send(newSd, &msg, sizeof(msg), 0);
	}

	//закрываем дескриптторы сокета
	close(newSd);
	close(serverSd);
	std::cout << "Connection closed" << std::endl;
	std::cout << "Byttes written: " << bytesWritten << std::endl;
	std::cout << "Byttes readen: " << bytesRead << std::endl;

	return 0;
}