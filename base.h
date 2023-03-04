//файл для базовых функций-обработчиков

int Socket(int domain, int type, int protocol)
{
	int res = socket(domain, type, protocol);
	if(res < 0)
	{
		std::cerr << "ERROR: establishing the server socket" << std::endl;
		exit(1);
	}
	return res;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int res = bind(sockfd, addr, addrlen);
	if(res < 0)
	{
		std::cerr << "ERROR: binding socket to local address" << std::endl;
		exit(1);
	}
	return res;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int res = accept(sockfd, addr, addrlen);
	if(res < 0)
	{
		std::cerr << "ERROR: accepting request from client" << std::endl;
		exit(1);
	}
	return res;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int res = connect(sockfd, addr, addrlen);
	if(res < 0)
	{
		std::cerr << "ERROR: connecting to socket" << std::endl;
		exit(1);
	}
	return res;
}