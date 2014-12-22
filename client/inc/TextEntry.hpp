#ifndef _TEXTENTRY_HPP_
# define _TEXTENTRY_HPP_

# include <SFML/Graphics.hpp>
# include <iostream>
# include <memory>
# include "ADrawable.hpp"
# include "Image.hpp"
# include "Text.hpp"

class TextEntry: public ADrawable
{
public:
  TextEntry(const std::string &placeHolder, const sf::FloatRect &pos, const std::shared_ptr<sf::Sprite> &back);
  ~TextEntry();

  void  setFont(const sf::Font &);
  void  setCharacterSize(unsigned int);
  void  setTextColor(const sf::Color &);
  const std::string &getText() const;
  void  setText(const std::string &text);
  
  void  onKey(sf::Keyboard::Key t, std::function<void ()> func);
  void  setUse(bool b);

  TextEntry &operator+=(char c);
  TextEntry &operator+=(const std::string &str);
  
  void  draw(sf::RenderWindow &win);
  void  update(const sf::Event &event, float timeElapsed);

private:
  bool                        _use;
  std::string                 _text;
  bool                        _first;
  bool                        _isHover;
  std::shared_ptr<Image>      _background;
  Text                        _displayText;
  std::string                 _placeHolder;
  std::map<sf::Keyboard::Key, std::function<void()>> _keyBinding;
};

#endif
