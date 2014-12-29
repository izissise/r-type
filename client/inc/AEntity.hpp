#ifndef _AENTITY_HPP_
# define _AENTITY_HPP_

# include "Vector.hpp"
# include "ADrawable.hpp"
# include "AnimatedSprites.hpp"

class AEntity : public ADrawable
{
public:
  AEntity(const Vector<float> &pos, const Vector<float> &size, uint8_t life, uint16_t score, const std::shared_ptr<AnimatedSprites> &sprite)
  : ADrawable(false, {pos.x, pos.y}, {size.x, size.y}), _life(life), _score(score), _sprite(sprite) {};
  virtual ~AEntity() {};
  
  uint8_t getLife() const { return _life; };
  void  setLife(uint8_t life) { _life = life; };

  uint16_t  getScore() const { return _score; };
  void  setScore(uint16_t score) { _score = score; };

  uint8_t getAnim() const { return _sprite->getCurrentAnim(); };
  void setAnim(uint8_t anim) const { return _sprite->setCurrentAnim(anim); };

protected:
  uint8_t                           _life;
  uint32_t                          _score;
  std::shared_ptr<AnimatedSprites>  _sprite;
};

#endif
