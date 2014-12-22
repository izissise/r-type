#include <algorithm>
#include <sstream>
#include "ListBox.hpp"

ListBox::ListBox(const sf::FloatRect &pos, std::vector<t_room> &vec, std::function<void (uint32_t)> func)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _list(vec), _cam(_pos.y), _func(func)
{
  if (!_list.empty())
    updateEntry();
}

ListBox::~ListBox()
{

}

void  ListBox::update(const sf::Event &event, float timeElapsed)
{
  updateEntry();
  for (auto it : _items)
    it.second->update(event, timeElapsed);
  if (event.type == sf::Event::MouseWheelMoved
      && event.mouseWheel.x >= _pos.x && event.mouseWheel.x < _pos.x + _size.x
      && event.mouseWheel.y >= _pos.y && event.mouseWheel.y < _pos.y + _size.y)
  {
    float tmp = _cam + (event.mouseWheel.delta * 2);

    if (tmp <= _pos.y && tmp >= (_pos.y + _size.y) - (_items.size() * 50) - _pos.y)
      _cam = tmp;
  }
}

void  ListBox::draw(sf::RenderWindow &win)
{
  float y = _cam;
  sf::View  view({_pos.x, _pos.y, _size.x, _size.y});

  view.setViewport({ _pos.x / win.getSize().x, _pos.y / win.getSize().y, _size.x / win.getSize().x, _size.y / win.getSize().y});
  win.setView(view);
  for (auto it : _items)
  {
    it.second->setPosition({_pos.x, y});
    it.second->draw(win);
    y += it.second->getSize().y;
  }
  win.setView(win.getDefaultView());
}

void  ListBox::updateEntry()
{
  if (!isSame())
  {
    _displayRoom.clear();
    _items.clear();
    _displayRoom = _list;

    for (auto &it : _displayRoom)
    {
      std::stringstream ss("");

      ss << static_cast<int>(it.nbPlayer) << " / " << static_cast<int>(it.playerMax);
      std::shared_ptr<Text>  name(new Text({0, 0, 100, 100}, it.name));
      std::shared_ptr<Text>  player(new Text({200, 0, 100, 100}, ss.str()));

      name->setCharacterSize(30);
      player->setCharacterSize(30);

      name->setColor(sf::Color::Black);
      player->setColor(sf::Color::Black);

      name->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
      player->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));

      auto texture = RessourceManager::instance().getTexture("../assets/ListEntry.png");

      std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
      std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
      std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));

      button->setTextureRect(sf::IntRect(0, 0, 480, 30));
      hover->setTextureRect(sf::IntRect(0, 30, 480, 30));
      click->setTextureRect(sf::IntRect(0, 60, 480, 30));

      std::shared_ptr<Button> tmp(new Button({ 0, 0 , _size.x, 50 }, button, hover, click));

      tmp->onClick([this, it] {
        _func(it.id);
        std::cout << "Join [" << it.name << "]" << std::endl;
      });
      _items[it.id] = std::shared_ptr<ListItem>(new ListItem({_size.x, 50},
                                                             name, player, tmp));
    }
  }
}

bool  ListBox::isSame() const
{
  auto first1 = _list.begin();
  auto first2 = _displayRoom.begin();

  for (; first1 != _list.end() && first2 != _displayRoom.end(); first1++, first2++) {
    if (!((*first1).id == (*first2).id && (*first1).nbPlayer == (*first2).nbPlayer))
      return false;
  }
  if (first1 == _list.end() && first2 == _displayRoom.end())
    return true;
  return false;
}