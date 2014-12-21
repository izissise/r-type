#ifndef _UDPCONNECTIONPROBLEM_HPP_
# define _UDPCONNECTIONPROBLEM_HPP_

# include <stdexcept>

class UDPConnectionProblem : public std::runtime_error
{
public:
  UDPConnectionProblem(const std::string &w);
};

#endif
