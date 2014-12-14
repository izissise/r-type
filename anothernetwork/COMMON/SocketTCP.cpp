#include	"SocketTCP.hpp"

SocketTCP::SocketTCP(int port) : ASocketOS(port)
{
	this->_type = SERVER;
	this->_proto = "tcp";
	this->_socktype = SOCK_STREAM;
	this->InitServer();
}

SocketTCP::SocketTCP(std::string ip, int port) : ASocketOS(ip, port)
{
	this->_type = CLIENT;
	this->_proto = "tcp";
	this->_socktype = SOCK_STREAM;
	this->InitClient();
}

bool		SocketTCP::InitServer()
{
	bool	ret = false;

	this->_ip = "NULL";
	ret = this->Socket();
	if (ret == false)
		std::cerr << "SOCKET TCP CREATE ERROR" << std::endl;
	else
	{
		bind(this->_fdSocket, (struct sockaddr *)&this->_sin, sizeof(sockaddr));
		listen(this->_fdSocket, 5);
	}
	std::cout << "SOCKET TCP INITIALIZE" << std::endl;
	return ret;
}

bool		SocketTCP::InitClient()
{
	bool	ret = false;

	ret = this->Socket();
	if (ret == false)
		std::cerr << "SOCKET TCP CREATE ERROR" << std::endl;
	else
	{
		ret = this->Connect();
		if (ret == false)
			std::cerr << "SOCKET TCP CONNECT ERROR" << std::endl;
	}
	std::cout << "SOCKET TCP INITIALIZE" << std::endl;
	return ret;
}

int		SocketTCP::Send(int id, void *msg, int len)
{
	std::map<int, Client>::iterator it;
	int	ret;

	for (it = this->_clients.begin(); it != this->_clients.end() && (*it).first != id; ++it);
	if (it == this->_clients.end() && this->_type == SERVER)
		return (IDERROR);
	if (this->_type != CLIENT)
		ret = this->SendTo(this->_clients[id].fd, msg, len);
	else
		ret = this->SendTo(this->_fdSocket, msg, len);
	if (ret == DISCONNECTED)
		this->_clients.erase(id);
	return ret;
}

int		SocketTCP::Receive(int id, void *msg, int len)
{
	std::map<int, Client>::iterator it;
	int	ret;

	for (it = this->_clients.begin(); it != this->_clients.end() && (*it).first != id; ++it);
	if (it == this->_clients.end() && this->_type == SERVER)
		return (IDERROR);
	else if (this->_type == SERVER && this->Select((*it).second.fd) == false)
		return (NOMSG);
	else if (this->_type == CLIENT && this->Select(this->_fdSocket) == false)
		return (NOMSG);
	if (this->_type == CLIENT)
		ret = this->ReceiveFrom(this->_fdSocket, msg, len);
	else
		ret = this->ReceiveFrom((*it).second.fd, msg, len);
	if (ret == DISCONNECTED)
		this->_clients.erase(it);
	return ret;
}

int		SocketTCP::Receive(void *msg, int len)
{
	std::map<int, Client>::iterator it;
	int	ret;

	for (it = this->_clients.begin(); it != this->_clients.end() && this->Select((*it).second.fd) == false; ++it);
	if (it == this->_clients.end() && this->_type == SERVER)
		return (NOMSG);
	else if (this->_type == CLIENT && this->Select(this->_fdSocket) == false)
		return (NOMSG);
	if (this->_type == CLIENT)
		ret = this->ReceiveFrom(this->_fdSocket, msg, len);
	else
		ret = this->ReceiveFrom((*it).second.fd, msg, len);

	if (ret <= DISCONNECTED)
	{
		if (ret == DISCONNECTED)
			this->_clients.erase(it);
		return ret;
	}
	if (this->_type == CLIENT)
		return OK;
	return (*it).first;
}