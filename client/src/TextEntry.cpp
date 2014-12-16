#include "TextEntry.hpp"

TextEntry::TextEntry(const std::string &placeHolder, const sf::FloatRect &pos, const std::shared_ptr<sf::Sprite> &back)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _use(false),
  _text(""), _first(true), _isHover(false), _background(new Image(back, pos)),
  _displayText(pos), _placeHolder(placeHolder)
{
}

TextEntry::~TextEntry()
{

}

void  TextEntry::setFont(const sf::Font &font)
{
  _displayText.setFont(font);
}

void  TextEntry::setCharacterSize(unsigned int size)
{
  _displayText.setCharacterSize(size);
}

void  TextEntry::setTextColor(const sf::Color &color)
{
  _displayText.setColor(color);
}

const std::string &TextEntry::getText() const
{
  return (_text);
}

TextEntry &TextEntry::operator+=(char c)
{
  _text += c;
  return (*this);
}

TextEntry &TextEntry::operator+=(const std::string &str)
{
  _text += str;
  return (*this);
}

void  TextEntry::draw(sf::RenderWindow &win)
{
  sf::View  text({_pos.x, _pos.y, _size.x, _size.y});

  text.setViewport(sf::FloatRect(_pos.x / win.getSize().x, _pos.y / win.getSize().y, _size.x / win.getSize().x, _size.y / win.getSize().y));
  win.setView(text);
  _background->setSize(_size);
  _background->setPosition({_pos.x, _pos.y});
  if (_first)
    _displayText.setString(_placeHolder);
  else
    _displayText.setString(_text);
  if (_displayText.getLocalBounds().width > _size.x)
    _displayText.setPosition({_pos.x + (_size.x - _displayText.getLocalBounds().width),
                              _pos.y + (_size.y / 2) - (_displayText.getLocalBounds().height / 2)});
  else
    _displayText.setPosition({_pos.x + (_size.x / 2) - (_displayText.getLocalBounds().width / 2),
                              _pos.y + (_size.y / 2) - (_displayText.getLocalBounds().height / 2)});
  _background->draw(win);
  _displayText.draw(win);
  win.setView(win.getDefaultView());
}

void  TextEntry::update(const sf::Event &event)
{
  if (event.type == sf::Event::MouseMoved)
  {
    if (event.mouseMove.x >= _pos.x && event.mouseMove.x < _pos.x + _size.x
        && event.mouseMove.y >= _pos.y && event.mouseMove.y < _pos.y + _size.y)
      _isHover = true;
    else
      _isHover = false;
  }
  if (_isHover && event.type == sf::Event::MouseButtonReleased)
  {
    _use = true;
    _first = false;
  }
  else if (!_isHover && event.type == sf::Event::MouseButtonReleased)
  {
    _use = false;
    if (_text.empty())
      _first = true;
  }
  if (_use && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && !_text.empty())
      _text.pop_back();
  if (_use && event.type == sf::Event::TextEntered && event.text.unicode > 20 && event.text.unicode < 128)
    _text += static_cast<char>(event.text.unicode);
}
