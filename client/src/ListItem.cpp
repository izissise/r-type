#include "ListItem.hpp"

ListItem::ListItem(const sf::Vector2f &size, const std::shared_ptr<Text> &name, const std::shared_ptr<Text> player, const std::shared_ptr<Button> &entry)
: ADrawable(false), _size(size), _name(name), _player(player), _entry(entry)
{
  
}

ListItem::~ListItem()
{
  
}

void ListItem::update(const sf::Event &event)
{
  _entry->update(event);
  _name->update(event);
  _player->update(event);
}

void ListItem::draw(sf::RenderWindow &win)
{
  _entry->draw(win);
  _name->draw(win);
  _player->draw(win);
}