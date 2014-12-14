#include	"SocketUDP.hpp"

SocketUDP::SocketUDP(int port) : ASocketOS(port)
{
	this->_type = SERVER;
	this->_proto = "udp";
	this->_socktype = SOCK_DGRAM;
	this->InitServer();
}

SocketUDP::SocketUDP(std::string ip, int port) : ASocketOS(ip, port)
{
	this->_type = CLIENT;
	this->_proto = "udp";
	this->_socktype = SOCK_DGRAM;
	this->InitClient();
}

bool		SocketUDP::InitServer()
{
	bool	ret = false;

	this->_ip = "NULL";
	ret = this->Socket();
	if (ret == false)
		std::cerr << "SOCKET UDP CREATE ERROR" << std::endl;
	else
		bind(this->_fdSocket, (struct sockaddr *)&this->_sin, sizeof(sockaddr));
	std::cout << "SOCKET UDP INITIALIZE" << std::endl;
	return ret;
}

bool		SocketUDP::InitClient()
{
	bool	ret = false;

	ret = this->Socket();
	if (ret == false)
		std::cerr << "SOCKET UDP CREATE ERROR" << std::endl;
	std::cout << "SOCKET UDP INITIALIZE" << std::endl;
	return ret;
}

int		SocketUDP::Send(int id, void *msg, int len)
{
	std::map<int, Client>::iterator it;
	int	ret;

	for (it = this->_clients.begin(); it != this->_clients.end() && (*it).first != id; ++it);
	if (it == this->_clients.end() && this->_type == SERVER)
		return (IDERROR);
	if (id == SERVER)
		ret = this->SendTo(&this->_clients[id].sin, msg, len);
	else
		ret = this->SendTo(&this->_sin, msg, len);
	return ret;
}

int		SocketUDP::Receive(void *msg, int len)
{
	SOCKADDR_IN	sintmp;
	Client		newclient;
	int	ret;

	if (this->Select(this->_fdSocket) == false)
		return (NOMSG);
	ret = this->ReceiveFrom(&sintmp, msg, len);
	if (ret <= DISCONNECTED)
		return ret;
	if (this->_type == CLIENT)
		return OK;
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it).second.sin.sin_addr.s_addr == sintmp.sin_addr.s_addr)
			return ((*it).first);
	}
	newclient.fd = -1;
	newclient.id = this->_clients.size() + 1;
	newclient.sin = sintmp;
	return newclient.id;
}