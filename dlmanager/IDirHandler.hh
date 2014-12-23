#ifndef IDIRHANDLER_HH_
# define	IDIRHANDLER_HH_

# include	<string>
# include	<list>

class IDirHandler
{
public:
	virtual std::list<std::string>	GetFilesFromDir(const std::string &dir) const = 0;
	virtual std::list<std::string>	GetLibsFromDir(const std::string &dir) const = 0;

private:

};

#endif // !IDIRHANDLER_HH_
