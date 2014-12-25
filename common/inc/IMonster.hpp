#ifndef IMONSTERS_HH_
# define IMONSTERS_HH_

# include <string>

class IMonster
{
public:
 virtual const std::string& getName() const = 0;
 virtual int getPower() const = 0;
 virtual int getLife() const = 0;

};

#endif // IMONSTERS_HH_

