#ifndef MONSTERS_HH_
# define	MONSTERS_HH_

# include	"IMonsters.hh"

# ifdef _WIN32
#  define	EXPORT _declspec(dllexport)
# else
#  define	EXPORT
#endif

class Monsters : public IMonsters
{
public:
	Monsters() {}
	~Monsters() {}
	const std::string	&GetName(void) const;
	int					GetPower(void) const;
	int					GetLife(void) const;

private:
	std::string	name = "B";
	int			power = 20;
	int			life = 20;
};

#endif // IMONSTERS_HH_