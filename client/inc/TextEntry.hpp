#ifndef _TEXTENTRY_HPP_
# define _TEXTENTRY_HPP_

# include <SFML/Graphics.hpp>
# include <iostream>

class TextEntry
{
public:
  TextEntry(const std::string &placeHolder, const sf::Vector2f &pos, const std::shared_ptr<sf::Sprite> &back);
  ~TextEntry();

  void  setFont(const sf::Font &);
  void  setCharacterSize(unsigned int);
  void  setTextColor(const sf::Color &);
  const std::string &getText() const;
  
  TextEntry &operator+=(char c);
  TextEntry &operator+=(const std::string &str);
  
  void  draw(sf::RenderWindow &win);
  void  update(const sf::Event &event);

private:
  sf::Vector2f                _pos;
  bool                        _use;
  std::string                 _text;
  bool                        _first;
  std::shared_ptr<sf::Sprite> _background;
  sf::Text                    _displayText;
  std::string                 _placeHolder;
};

#endif
