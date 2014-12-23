#include	"Monsters.hh"

const std::string	&Monsters::GetName(void) const
{
	return this->name;
}

int					Monsters::GetPower(void) const
{
	return this->power;
}

int					Monsters::GetLife(void) const
{
	return this->life;
}

extern "C" EXPORT	IMonsters*	Create()
{
	return (new Monsters());
}