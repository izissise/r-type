#include	"DLManager.hh"

template<typename T>
DLManager<T>::DLManager(const std::string &rep)
{}

template<typename T>
DLManager<T>::~DLManager()
{
	if (this->modules)
		this->modules.clear();
}

template<typename T>
const T	&DLManager<T>::GetInstance(const std::string &name)
{
	return this->modules[name].GetInstance();
}

template<typename T>
bool	DLManager<T>::LoadFromRep()
{
	return false;
}