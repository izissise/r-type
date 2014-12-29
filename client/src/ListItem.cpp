#include "ListItem.hpp"

ListItem::ListItem(const sf::Vector2f &size, const std::shared_ptr<Text> &name, const std::shared_ptr<Text> player, const std::shared_ptr<Button> &entry)
: ADrawable(false, {0, 0}, size), _size(size), _name(name), _player(player), _entry(entry)
{
  
}

ListItem::~ListItem()
{
  
}

void ListItem::update(const Input &event, float timeElapsed)
{
  _entry->update(event, timeElapsed);
  _name->update(event, timeElapsed);
  _player->update(event, timeElapsed);
}

void ListItem::draw(sf::RenderWindow &win)
{
  _entry->setSize(_size);
  _entry->setPosition(_pos);
  _entry->draw(win);

  _name->setPosition({_pos.x + 5, _pos.y});
  _name->draw(win);

  _player->setPosition({_pos.x + _size.x - _player->getLocalBounds().width - 10, _pos.y});
  _player->draw(win);
}