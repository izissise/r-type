#include	"ASocket.hpp"

bool		ASocket::Select(SOCKET fd)
{
  fd_set			fd_read;
  struct timeval	tv;

  tv.tv_sec = 0;
  tv.tv_usec = 5000;

  FD_ZERO(&fd_read);
  FD_SET(fd, &fd_read);
  if (select(fd + 1, &fd_read, NULL, NULL, &tv) == -1)
    return false;
  if (FD_ISSET(fd, &fd_read))
    return true;
  return false;
}