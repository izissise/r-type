#ifndef _TEXT_HPP_
# define _TEXT_HPP_

# include <SFML/Graphics.hpp>
# include "ADrawable.hpp"

class Text: public ADrawable
{
public:
  Text(const std::string &text = "");
  ~Text();
  
  void  move(float x, float y);
  
  void  setCharacterSize(unsigned int);
  void  setString(const std::string &);
  void  setPosition(const sf::Vector2f &);
  void  setFont(const sf::Font &);
  void  setColor(const sf::Color &c) const;

  const std::string getString() const;
  const sf::FloatRect getLocalBounds() const;
  
  virtual void update(const sf::Event &);
  virtual void draw(sf::RenderWindow &win);

private:
  std::shared_ptr<sf::Text> _text;
};

#endif
