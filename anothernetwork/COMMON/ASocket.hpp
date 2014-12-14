#ifndef ASOCKET_HPP_
# define	ASOCKET_HPP_

# ifdef	_WIN32
#  pragma comment(lib,"Ws2_32.lib")
#  include <WinSock2.h>
typedef int socklen_t;
#elif	__unix
#  include	<sys/types.h>
#  include	<sys/socket.h>
#  include	<netinet/in.h>
#  include	<arpa/inet.h>
#  include	<unistd.h>
# include	<netdb.h>
#  define	INVALID_SOCKET -1
#  define	SOCKET_ERROR -1
#  define	closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

# include	<stdio.h>
# include	<stdlib.h>
# include	<string>
# include	<iostream>
# include	<ostream>
# include	<map>

enum Return
{
	IDERROR = -3,
	NOMSG = -2,
	RETERROR = -1,
	DISCONNECTED,
	OK
};

enum Type
{
	SERVER = 0,
	CLIENT
};

typedef struct Client
{
	SOCKET		fd;
	SOCKADDR_IN	sin;
	int			id;
}	Client;

class ASocket
{
public:
	ASocket(int port) : _port(port) {}
	ASocket(const std::string &ip, int port) : _ip(ip), _port(port) {}
	virtual	~ASocket() {}
	virtual int			Accept() = 0;
	virtual int			Send(int id, void *msg, int bits) = 0;
	virtual int			Receive(void *msg, int bits) = 0;
	virtual int			Receive(int id, void *msg, int bits) = 0;
	virtual bool		Close(SOCKET fd) = 0;

protected:
	std::string				_ip;
	std::string				_proto;
	int						_port;
	SOCKADDR_IN				_sin;
	SOCKET					_fdSocket;
	Type					_type;
	int						_socktype;
	std::map<int, Client>	_clients;

	virtual bool		InitServer() = 0;
	virtual bool		InitClient() = 0;
	virtual bool		Socket() = 0;
	virtual bool		Connect() = 0;
	virtual int			SendTo(SOCKET fd, void *msg, int bits) = 0;
	virtual int			ReceiveFrom(SOCKET fd, void *msg, int bits) = 0;
	virtual int			SendTo(SOCKADDR_IN *sin, void *msg, int bits) = 0;
	virtual int			ReceiveFrom(SOCKADDR_IN *sin, void *msg, int bits) = 0;
	bool				Select(SOCKET fd);

private:
};

#endif