#ifndef EXAMPLEMONSTERS_HH_
# define EXAMPLEMONSTERS_HH_

# include "RTypeConfig.h"
# include "megaBlorbMonster_Export.h"

# include "ILibMonster.hpp"

extern "C"
{
megaBlorbMonster_EXPORT ILibMonster* createMonster();
}

class MegaBlorbMonster : public ILibMonster
{
public:
 MegaBlorbMonster() = default;
 virtual ~MegaBlorbMonster() = default;
 const std::string& getName() const;
 int getPower() const;
 int getLife() const;

private:
 std::string _name = "megaBlorbMonster";
 int         _power = 10;
 int         _life = 10;
};

#endif // IMONSTERS_HH_