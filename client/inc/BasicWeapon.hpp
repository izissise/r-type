#ifndef _BASICWEAPON_HPP_
# define _BASICWEAPON_HPP_

# include "AWeapon.hpp"

class BasicWeapon : public AWeapon
{
public:
    BasicWeapon(const std::shared_ptr<AnimatedSprites> &sprite);
    ~BasicWeapon();

    virtual void update(const Input &, float) override;
    virtual void draw(sf::RenderWindow &win) override;
};

#endif