#ifndef _FACTORY_HPP_
# define _FACTORY_HPP_

# include "SocketTCP.hpp"
# include "SocketUDP.hpp"

class Factory
{
	public:
		template<typename T, typename... Args>
		static T *CreateSocketTCP(Args... ar)
		{
			return ((T*)new SocketTCP(ar...));
		}

		template<typename T, typename... Args>
		static T *CreateSocketUDP(Args... ar)
		{
			return ((T*)new SocketUDP(ar...));
		}
};

# endif /* !_FACTORY_HPP_ */