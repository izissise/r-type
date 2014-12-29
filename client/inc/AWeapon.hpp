#ifndef _AWEAPON_HPP_
# define _AWEAPON_HPP_

# include "Vector.hpp"
# include "AEntity.hpp"

struct t_ammo
{
  t_ammo(const Vector<float> &p, const Vector<float> &sp, const std::shared_ptr<AnimatedSprites> &s)
  : pos(p), speed(sp), sprite(s)
  {};
  Vector<float> pos;
  Vector<float> speed;
  std::shared_ptr<AnimatedSprites> sprite;
};

class AWeapon : public AEntity
{
public:
  AWeapon(const Vector<float> &pos, const Vector<float> &speed, int damage, const std::shared_ptr<AnimatedSprites> &sprite)
  : AEntity(pos, Vector<float>(sprite->getSize().x, sprite->getSize().y), 0, 0, sprite), _damage(damage), _speed(speed) {};
  virtual ~AWeapon() {};
  
  uint16_t getDamage() const { return _damage; };
  virtual t_ammo fire(const Vector<float> &pos) { return t_ammo(pos, _speed, _sprite); };
protected:
  uint16_t      _damage;
  Vector<float> _speed;
};

#endif
