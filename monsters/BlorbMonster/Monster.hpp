#ifndef EXAMPLEMONSTERS_HH_
# define EXAMPLEMONSTERS_HH_

# include "RTypeConfig.h"
# include "blorbMonster_Export.h"

# include "ILibMonster.hpp"

extern "C"
{
blorbMonster_EXPORT ILibMonster* createMonster();
}

class BlorbMonster : public ILibMonster
{
public:
 BlorbMonster() = default;
 virtual ~BlorbMonster() = default;
 const std::string& getName() const;
 int getPower() const;
 int getLife() const;

private:
 std::string _name = "blorbMonster";
 int         _power = 10;
 int         _life = 10;
};

#endif // IMONSTERS_HH_