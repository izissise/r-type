#ifndef _AWEAPON_HPP_
# define _AWEAPON_HPP_

# include <iostream>
# include "Vector.hpp"
# include "AEntity.hpp"

struct t_ammo
{
  t_ammo(const Vector<float> &p, const Vector<float> &sp, int d, const std::shared_ptr<AnimatedSprites> &s)
  : pos(p), speed(sp), damage(d), sprite(s)
  {};
  Vector<float> pos;
  Vector<float> speed;
  int			damage;
  int			anim;
  std::shared_ptr<AnimatedSprites> sprite;
};

class AWeapon : public AEntity
{
public:
  AWeapon(const Vector<float> &pos, const Vector<float> &speed, int damage, const std::shared_ptr<AnimatedSprites> &sprite)
	  : AEntity(pos, Vector<float>(sprite->getSize().x, sprite->getSize().y), 0, 0, sprite),
	  _damage(damage), _reload(std::chrono::steady_clock::now()), _speed(speed) { };
  virtual ~AWeapon() {};
  
  uint16_t getDamage() const { return _damage; };
  virtual t_ammo fire(const Vector<float> &pos)
  { 
	  if ((std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - _reload)).count() > 0.5)
	  {
		  _reload = std::chrono::steady_clock::now();
		  return t_ammo(pos, _speed, _damage, _sprite);
	  }
	  throw std::runtime_error("Cannot fire anymore");
  };
protected:
  uint16_t      _damage;
  std::chrono::steady_clock::time_point _reload;
  Vector<float> _speed;
};

#endif
