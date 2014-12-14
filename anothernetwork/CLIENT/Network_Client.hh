#ifndef _NETWORK_CLIENT_HH_
# define _NETWORK_CLIENT_HH_

# include	"../Common/Factory.hpp"
# include	"../Common/ASocket.hpp"

class NetworkClient
{
public:
	NetworkClient(const std::string &ip, int port);
	~NetworkClient();
	bool	Initialize();
	bool	Try_connect();
	int		Send(void *msg, int bits);
	int		Receive(void *msg, int bits);
	int		Receive(int id, void *msg, int nbr);

protected:
private:
	ASocket		*_socket;
	std::string	_ip;
	int			_port;
};

#endif
