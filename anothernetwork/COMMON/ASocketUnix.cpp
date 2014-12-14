#ifdef __unix__

# include	"ASocketUnix.hpp"
# include <netdb.h>

ASocketUnix::ASocketUnix(int port) : ASocket(port)
{
	this->_ip = "NULL";
	std::cout << "SERVER UNIX" << std::endl;
}

ASocketUnix::ASocketUnix(std::string ip, int port) : ASocket(ip, port)
{
	std::cout << "CLIENT UNIX" << std::endl;
}

ASocketUnix::~ASocketUnix()
{
	this->Close(this->_fdSocket);
}

bool	ASocketUnix::Socket()
{
	struct protoent	*proto;
	int ret;

	proto = getprotobyname(this->_proto.c_str());
	ret	= socket(AF_INET, this->_socktype, proto->p_proto);
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

bool	ASocketUnix::Connect()
{
	if (connect(this->_fdSocket, (struct sockaddr *)&this->_sin, sizeof(this->_sin)) == -1)
	{
		this->Close(this->_fdSocket);
		return false;
	}
	return true;
}

int		ASocketUnix::Accept()
{
	Client		newclient;
	SOCKADDR_IN	csin;
	int	cfd;
	int	clientsize = sizeof(csin);

	if (this->Select(this->_fdSocket) == false)
		return 0;
	cfd = accept(this->_fdSocket, (SOCKADDR*)&csin, &clientsize);
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

Return	ASocketUnix::SendTo(SOCKET fd, void *msg, int bits)
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
	return (NEWMSG);
}

Return	ASocketUnix::ReceiveFrom(SOCKET fd, void *msg, int bits)
{
	int	ret;

	ret = recv(fd, (char *)msg, bits, 0);
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
	{
		this->Close(fd);
		return (DISCONNECTED);
	}
	return (NEWMSG);
}

Return	ASocketUnix::SendTo(SOCKADDR_IN *sin, void *msg, int bits)
{
	int	ret;

	ret = sendto(this->_fdSocket, (char *)msg, bits, 0, (SOCKADDR *)sin, sizeof(*sin));
	if (ret == INVALID_SOCKET)
		return (RETERROR);
	else if (ret == 0)
		return (DISCONNECTED);
	return (OK);
}

Return	ASocketUnix::ReceiveFrom(SOCKADDR_IN *sin, void *msg, int bits)
{
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

bool	ASocketUnix::Close(SOCKET fd)
{
	if (close(fd) == -1)
		return false;
	return true;
}

#endif
