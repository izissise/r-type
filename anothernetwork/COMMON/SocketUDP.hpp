#ifndef		SOCKETUDP_HPP_
# define	SOCKETUDP_HPP_

# include	"ASocketWin.hpp"
# include	"ASocketUnix.hpp"

class SocketUDP : public ASocketOS
{
public:
	SocketUDP(int port);
	SocketUDP(std::string ip, int port);
	~SocketUDP() {}
	int				Send(int id, void *msg, int bits);
	int				Receive(void *msg, int bits);
	int				Receive(int id, void *msg, int nbr) { return 0; }


protected:
	bool			InitServer();
	bool			InitClient();

private:
};

#endif