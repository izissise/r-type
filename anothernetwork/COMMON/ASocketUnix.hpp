#ifdef __unix__
# ifndef	ASOCKETUNIX_HPP_
# define	ASOCKETUNIX_HPP_

# include	"ASocket.hpp"
# include	<typeinfo>

typedef class ASocketUnix : public ASocket
{
public:
	ASocketUnix(int port);
	ASocketUnix(std::string ip, int port);
	~ASocketUnix();
	int					Accept();
	bool				Close(SOCKET fd);
	virtual Return		Send(int id, void *msg, int bits) = 0;
	virtual Return		Receive(void *msg, int bits) = 0;
	virtual int			Receive(int id, void *msg, int nbr) = 0;

protected:
	virtual bool		InitServer() = 0;
	virtual bool		InitClient() = 0;
	bool				Socket();
	bool				Connect();
	Return				SendTo(SOCKET fd, void *msg, int bits);
	Return				ReceiveFrom(SOCKET fd, void *msg, int bits);
	Return				SendTo(SOCKADDR_IN *sin, void *msg, int bits);
	Return				ReceiveFrom(SOCKADDR_IN *sin, void *msg, int bits);

private:
}	SocketOS;

# endif
#endif