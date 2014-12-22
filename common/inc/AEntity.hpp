#ifndef _AENTITY_HPP_
# define _AENTITY_HPP_

# include "Vector.hpp"

class AEntity
{
public:
  AEntity(const Vector<float> &pos): _pos(pos) {};
  virtual ~AEntity() {};
  
  void  setPos(const Vector<float> &pos) { _pos = pos; };
  const Vector<float> &getPos() { return _pos; };
  
protected:
  Vector<float> _pos;
};

#endif
