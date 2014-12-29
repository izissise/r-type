#ifndef _Player_HPP_
# define _Player_HPP_

# include <cstdint>
# include <memory>
# include "AEntity.hpp"
# include "Vector.hpp"
# include "AWeapon.hpp"

class Player : public AEntity
{
public:
  enum Animation{
    DOWN = 0,
    MIDDOWN = 1,
    NORMAL = 2,
    MIDUP = 3,
    UP = 4,
  };

public:
  Player(const Vector<float> &pos, const Vector<float> &speed, const std::shared_ptr<AnimatedSprites> &sprites, const std::shared_ptr<AWeapon> &weapon);
  ~Player();
  
  const Vector<float> &getSpeed() const;
  void  setSpeed(const Vector<float> &);
  
  virtual void update(const Input &, float) override;
  virtual void draw(sf::RenderWindow &win) override;
  
  const std::shared_ptr<AWeapon> &getWeapon() const;
  void setWeapon(const std::shared_ptr<AWeapon> &weapon);
  
  void move(uint8_t axis, float time);
  void fire();
  
private:
  Vector<float> _speed;
  std::shared_ptr<AWeapon>  _weapon;
  std::vector<t_ammo>  _ammo;
};

#endif