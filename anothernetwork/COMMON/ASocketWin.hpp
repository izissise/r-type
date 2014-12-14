#ifdef _WIN32

# ifndef	ASOCKETWIN_HPP_
# define	ASOCKETWIN_HPP_

# include	"ASocket.hpp"

typedef class ASocketWin : public ASocket
{
public:
	ASocketWin(int port);
	ASocketWin(const std::string &ip, int port);
	~ASocketWin();
	int					Accept();
	bool				Close(SOCKET fd);
	virtual int			Send(int id, void *msg, int bits) = 0;
	virtual int			Receive(void *msg, int bits) = 0;
	virtual int			Receive(int id, void *msg, int nbr) = 0;


protected:
	virtual bool		InitServer() = 0;
	virtual bool		InitClient() = 0;
	bool				Socket();
	bool				Connect();
	int					SendTo(SOCKET fd, void *msg, int bits);
	int					ReceiveFrom(SOCKET fd, void *msg, int bits);
	int					SendTo(SOCKADDR_IN *sin, void *msg, int bits);
	int					ReceiveFrom(SOCKADDR_IN *sin, void *msg, int bits);
}	ASocketOS;

# endif
#endif