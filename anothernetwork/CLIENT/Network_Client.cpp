#include	"Network_Client.hh"

NetworkClient::NetworkClient(const std::string &ip, int port) : _ip(ip), _port(port) {}

NetworkClient::~NetworkClient() {}

bool	NetworkClient::Initialize()
{
	if (this->_socket)
	{
		std::cout << "Network WARNING: socket allready connected" << std::endl;
		return true;
	}
	this->_socket = Factory::CreateSocketTCP<ASocket>(this->_ip, this->_port);
	if (this->_socket == NULL)
	{
		std::cerr << "Network ERROR: can't connect to server" << std::endl;
		return false;
	}
	return true;
}

bool	NetworkClient::Try_connect()
{
	return true;
}

int		NetworkClient::Send(void *msg, int nbr)
{
	int	ret;

	ret = this->_socket->Send(NULL, msg, nbr);
	return ret;
}

int		NetworkClient::Receive(int id, void *msg, int nbr)
{
	int	ret;

	ret = this->_socket->Receive(id, msg, nbr);
	if (ret > DISCONNECTED)
		std::cout << "receive " << msg << "from : " << id << std::endl;
	return ret;
}

int		NetworkClient::Receive(void *msg, int nbr)
{
	int	ret;

	ret = this->_socket->Receive(msg, nbr);
	if (ret > DISCONNECTED)
		std::cout << "receive " << msg << "from : " << ret << std::endl;
	return ret;
}