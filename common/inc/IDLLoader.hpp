#ifndef IDLLOADER_HH_
# define	IDLLOADER_HH_

# include	<string>

template<typename T>
class IDLLoader
{
public:
	virtual T *GetInstance(void) = 0;

protected:
private:
};

#endif // !IDLLOADER_HH_
