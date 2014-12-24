#ifndef IMONSTERS_HH_
# define IMONSTERS_HH_

# include <string>

class IMonsters
{
public:
 virtual const std::string &GetName(void) const = 0;
 virtual int     GetPower(void) const = 0;
 virtual int     GetLife(void) const = 0;

private:

};

#endif // IMONSTERS_HH_

