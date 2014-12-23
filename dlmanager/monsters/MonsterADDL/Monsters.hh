#ifndef MONSTERS_HH_
# define	MONSTERS_HH_

# include	"IMonsters.hh"

class Monsters : public IMonsters
{
public:
	Monsters() {}
	~Monsters() {}
	const std::string	&GetName(void) const;
	int					GetPower(void) const;
	int					GetLife(void) const;

private:
	std::string	name = "A";
	int			power = 10;
	int			life = 10;
};

#endif // IMONSTERS_HH_