#include	"Network_Serveur.hh"

NetworkServer::NetworkServer(int port) : _port(port) {}

NetworkServer::~NetworkServer() {}

bool	NetworkServer::Initialize()
{
	if (this->_socket)
	{
		std::cout << "Network WARNING: socket allready created" << std::endl;
		return true;
	}
	this->_socket = Factory::CreateSocketUDP<ASocket>(this->_port);
	if (this->_socket == NULL)
	{
		std::cout << "NETWORK ERROR: can't connect server" << std::endl;
		return false;
	}
	return true;
}

int	NetworkServer::Send(int id, void *msg, int nbr)
{
	int	ret = NOMSG;

	ret = this->_socket->Send(id, msg, nbr);
	return ret;
}

int	NetworkServer::Receive(int id, void *msg, int nbr)
{
	int	ret = NOMSG;

	ret = this->_socket->Receive(id, msg, nbr);
	if (ret > DISCONNECTED)
		std::cout << "receive " << (char *)msg << " from : " << id << std::endl;
	return ret;
}

int	NetworkServer::Receive(void *msg, int nbr)
{
	int	ret = NOMSG;

	ret = this->_socket->Receive(msg, nbr);
	if (ret > DISCONNECTED)
		std::cout << "receive " << (char *)msg << " from : " << ret << std::endl;
	return ret;
}

int		NetworkServer::NewClient()
{
	int	idtmp;

	idtmp = this->_socket->Accept();
	if (idtmp)
	{
		std::cout << "NEW CLIENT ID : " << idtmp << std::endl;
	}
	return idtmp;
}

void	NetworkServer::DeleteClient(int id)
{}
