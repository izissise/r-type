#ifdef __unix__
# include	"DirHandlerUnix.hh"

IDirHandler				*DirHandler::GetInstance()
{
	if (!this->instance)
		this->instance = new DirHandler();
	return (this->instance);
}

const std::list<std::string>	&DirHandler::GetFilesFromDir(const std::string &dir) const
{
	std::list<std::string>	list;
	return (list);
}

const std::list<std::string>	&DirHandler::GetLibsFromDir(const std::string &dir) const
{
	std::list<std::string>	list;
	return (list);
}

#endif // __unix__
