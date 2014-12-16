#ifndef _BUTTON_HPP_
# define _BUTTON_HPP_

# include <SFML/Graphics.hpp>
# include <functional>
# include <iostream>
# include <memory>
# include "ADrawable.hpp"
# include "Image.hpp"
# include "Text.hpp"

class Button: public ADrawable
{
public:
  Button(const sf::FloatRect &pos,
         const std::shared_ptr<sf::Sprite> &displayTexture,
         const std::shared_ptr<sf::Sprite> &hoverTexture,
         const std::shared_ptr<sf::Sprite> &clickedTexture,
         const std::shared_ptr<Text> &text);
  Button(const sf::FloatRect &pos,
         const std::shared_ptr<sf::Sprite> &displayTexture,
         const std::shared_ptr<sf::Sprite> &hoverTexture,
         const std::shared_ptr<sf::Sprite> &clickedTexture);
  ~Button();
  
  void  update(const sf::Event &event);
  void  draw(sf::RenderWindow &win);
  
  bool  isHover() const;
  bool  isClicked() const;
  void  onClick(const std::function<void ()> &func);

private:

  bool  _hover;
  bool  _isClicked;

  sf::FloatRect               _pos;
  std::function<void ()>      _onClick;
  std::shared_ptr<Image>      _hoverTexture;
  std::shared_ptr<Image>      _displayTexture;
  std::shared_ptr<Image>      _clickedTexture;
  std::shared_ptr<Text>       _text;
};

#endif
