#include <algorithm>
#include <sstream>
#include "GameMessageBox.hpp"

GameMessageBox::GameMessageBox(const sf::FloatRect &pos, const std::vector<std::string> &vec, bool focus)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _vec(vec), _focus(focus), _cam(_pos.y)
{
  if (!_vec.empty())
    updateEntry();
}

GameMessageBox::~GameMessageBox()
{

}

void  GameMessageBox::update(const Input &event, float timeElapsed)
{
  auto mousePos = event.getMousePos();
  updateEntry();
  for (auto &it : _text)
    it.update(event, timeElapsed);
  if (event.isMouseWheeled()
      && mousePos.x >= _pos.x && mousePos.x < _pos.x + _size.x
      && mousePos.y >= _pos.y && mousePos.y < _pos.y + _size.y)
  {
    float tmp = _cam + (event.getWheel() * 2);

    if (tmp <= _pos.y && tmp >= (_pos.y + _size.y) - (_text.size() * 40) - _pos.y)
      _cam = tmp;
  }
}

void  GameMessageBox::draw(sf::RenderWindow &win)
{
  float y = _cam;

  if (_focus)
    y = (_pos.y + _size.y) - (_text.size() * 40) - _pos.y;

  sf::View  view({_pos.x, _pos.y, _size.x, _size.y});

  view.setViewport({ _pos.x / win.getSize().x, _pos.y / win.getSize().y, _size.x / win.getSize().x, _size.y / win.getSize().y});
  win.setView(view);
  for (auto &it : _text)
  {
    it.setPosition({_pos.x, y});
    it.draw(win);
    y += it.getSize().y;
  }
  win.setView(win.getDefaultView());
}

void  GameMessageBox::updateEntry()
{
  if (!isSame())
  {
    _text.clear();

    for (auto &it : _vec)
    {
      Text  text({0, 0, _size.x, 40}, it);

      text.setCharacterSize(30);
      text.setColor(sf::Color::White);
      text.setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
      _text.push_back(text);
    }
  }
}

bool  GameMessageBox::isSame() const
{
  auto first1 = _vec.begin();
  auto first2 = _text.begin();

  for (; first1 != _vec.end() && first2 != _text.end(); first1++, first2++) {
    if (!((*first1) == (*first2).getString()))
      return false;
  }
  if (first1 == _vec.end() && first2 == _text.end())
    return true;
  return false;
}