#ifdef _WIN32

# include	"ASocketWin.hpp"

ASocketWin::ASocketWin(int port) : ASocket(port)
{
	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->_ip = "NULL";
	std::cout << "SERVER WINDOWS" << std::endl;
}

ASocketWin::ASocketWin(const std::string &ip, int port) : ASocket(ip, port)
{
	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	std::cout << "CLIENT WINDOWS" << std::endl;
}

ASocketWin::~ASocketWin()
{
	WSACleanup();
	this->Close(this->_fdSocket);
}

bool	ASocketWin::Socket()
{
	struct protoent	*proto;
	int ret;

	proto = getprotobyname(this->_proto.c_str());
	if (proto == NULL)
	{
		std::cout << "ERROR GETPROTOBYNAME" << std::endl;
		return false;
	}
	ret	= WSASocket(AF_INET, this->_socktype, proto->p_proto, NULL, 0, 0);
	this->_fdSocket = ret;
	if (this->_fdSocket == INVALID_SOCKET)
		return false;
	this->_sin.sin_family = AF_INET;
	this->_sin.sin_port = htons(this->_port);
	if (this->_ip == "NULL")
		this->_sin.sin_addr.s_addr = INADDR_ANY;
	else
		this->_sin.sin_addr.s_addr = inet_addr(this->_ip.c_str());
	return true;
}

bool	ASocketWin::Connect()
{
	if (WSAConnect(this->_fdSocket, (struct sockaddr *)&this->_sin, sizeof(this->_sin), 0, 0, 0, 0) != 0)
	{
		this->Close(this->_fdSocket);
		return false;
	}
	return true;
}

int		ASocketWin::Accept()
{
	Client		newclient;
	SOCKADDR_IN	csin;
	int	cfd;
	int	clientsize = sizeof(csin);

	if (this->Select(this->_fdSocket) == false)
		return 0;
	cfd = WSAAccept(this->_fdSocket, (SOCKADDR*)&csin, &clientsize, NULL, NULL);
	if (cfd == INVALID_SOCKET)
	{
		std::cerr << "SOCKET ACCEPT ERROR" << std::endl;
		return (0);
	}
	 newclient.fd = cfd;
	 newclient.sin = csin;
	 newclient.id = this->_clients.size() + 1;
	 _clients.insert(std::pair<int, Client>(newclient.id, newclient));
	 return newclient.id;
}

int	ASocketWin::SendTo(SOCKET fd, void *msg, int bits)
{
	int	ret;

	ret = send(fd, (char *)msg, bits, 0);
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
	{
		this->Close(fd);
		return (DISCONNECTED);
	}
	return (OK);
}

int	ASocketWin::ReceiveFrom(SOCKET fd, void *msg, int bits)
{
	/*if (this->Select(fd) == false)
		return (NOMSG);*/
	int	ret;

	ret = recv(fd, (char *)msg, bits, 0);
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
	{
		this->Close(fd);
		return (DISCONNECTED);
	}
	return (OK);
}

int	ASocketWin::SendTo(SOCKADDR_IN *sin, void *msg, int bits)
{
	int	ret;

	std::cout << (char *)msg << std::endl;
	ret = sendto(this->_fdSocket, (char *)msg, bits, 0, (SOCKADDR *)sin, sizeof(*sin));
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
		return (DISCONNECTED);
	return (OK);
}

int	ASocketWin::ReceiveFrom(SOCKADDR_IN *sin, void *msg, int bits)
{
	/*if (this->Select(this->_fdSocket) == false)
		return (NOMSG);*/
	int	ret;
	int	len;

	len = sizeof(*sin);
	ret = recvfrom(this->_fdSocket, (char *)msg, bits, 0, (SOCKADDR *)sin, &len);
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
		return (DISCONNECTED);
	return (OK);
}

bool	ASocketWin::Close(SOCKET fd)
{
	if (closesocket(fd))
		return false;
	return true;
}

#endif
