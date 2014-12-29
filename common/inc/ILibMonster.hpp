#ifndef AMONSTERS_HH_
# define AMONSTERS_HH_

# include <string>

class ILibMonster
{
public:
 virtual const std::string& getName() const = 0;
 virtual int getPower() const = 0;
 virtual int getLife() const = 0;

};

#endif // IMONSTERS_HH_

