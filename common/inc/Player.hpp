#ifndef _Player_HPP_
# define _Player_HPP_

# include <cstdint>
# include "AEntity.hpp"
# include "Vector.hpp"

class Player : public AEntity
{
public:
  enum class Animation{
    DOWN = 0,
    MIDDOWN = 1,
    NORMAL = 2,
    MIDUP = 3,
    UP = 4,
  };

public:
  Player(const Vector<float> &pos, const Vector<float> &speed);
  ~Player();
  
  const Vector<float> &getSpeed() const;
  void  setSpeed(const Vector<float> &);

  uint8_t getLife() const;
  void  setLife(uint8_t life);

  uint16_t  getScore() const;
  void  setScore(uint16_t score);
  
private:
  Vector<float> _speed;
  uint8_t       _life;
  uint16_t      _score;
};

#endif