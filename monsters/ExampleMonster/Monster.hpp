#ifndef EXAMPLEMONSTERS_HH_
# define EXAMPLEMONSTERS_HH_

# include "RTypeConfig.h"

# include "AMonster.hpp"

extern "C"
{
DLLAPI AMonster* createMonster();
}

class ExampleMonster : public AMonster
{
public:
 ExampleMonster() = default;
 virtual ~ExampleMonster() = default;
 const std::string& getName() const;
 int getPower() const;
 int getLife() const;

private:
 std::string _name = "Example";
 int         _power = 10;
 int         _life = 10;
};

#endif // IMONSTERS_HH_