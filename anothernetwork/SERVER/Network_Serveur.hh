#ifndef		NETWORK_SERVER_HH_
# define	NETWORK_SERVER_HH_

# include	"../Common/Factory.hpp"
# include	<map>

class NetworkServer
{
public:
	NetworkServer(int port);
	~NetworkServer();
	bool	Initialize();
	int		Send(int id, void *msg, int nbr);
	int		Receive(int id, void *msg, int nbr);
	int		Receive(void *msg, int nbr);
	int    	NewClient();
	void	DeleteClient(int id);

protected:
private:
	ASocket				*_socket;
	int					_port;
};

#endif
