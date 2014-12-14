#ifndef		SOCKETTCP_HPP_
# define	SOCKETTCP_HPP_

# include	"ASocketWin.hpp"
# include	"ASocketUnix.hpp"

class SocketTCP : public ASocketOS
{
public:
	SocketTCP(int port);
	SocketTCP(std::string ip, int port);
	~SocketTCP() {}
	int				Send(int id, void *msg, int bits);
	int				Receive(void *msg, int bits);
	int				Receive(int id, void *msg, int nbr);

protected:
	bool			InitServer();
	bool			InitClient();

private:
};

#endif