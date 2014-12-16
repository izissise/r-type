#include <sstream>
#include "ListBox.hpp"

ListBox::ListBox(const sf::FloatRect &pos, std::vector<t_room> &vec)
: _pos(pos), _list(vec)
{
  if (!_list.empty())
    updateEntry();
}

ListBox::~ListBox()
{
  
}

void  ListBox::update(const sf::Event &event)
{
  if (!std::is_permutation(_displayRoom.begin(), _displayRoom.end(), _list.begin(), [](const t_room &a, const t_room &b) -> bool { return a.id == b.id; }))
    updateEntry();
}

void  ListBox::draw(sf::RenderWindow &win)
{

}

void  ListBox::updateEntry()
{
  std::vector<t_room> toAdd;
  std::vector<t_room> toRemove;
  
  std::set_difference(_list.begin(), _list.end(), _displayRoom.begin(), _displayRoom.end(),
                      std::inserter(toAdd, toAdd.begin()), [](const t_room &a, const t_room &b) -> bool { return a.id == b.id; });
  
  std::set_difference(_displayRoom.begin(), _displayRoom.end(), _list.begin(), _list.end(),
                      std::inserter(toRemove, toRemove.begin()), [](const t_room &a, const t_room &b) -> bool { return a.id == b.id; });

  for (auto it : toAdd)
  {
    std::stringstream ss("");

    ss << it.nbPlayer << " / " << it.playerMax;
    Text  name(it.name);
    Text  player(ss.str());
    
    name.setCharacterSize(30);
    player.setCharacterSize(30);
    
    auto texture = RessourceManager::instance().getTexture("../assets/ListEntry.png");
    
    std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
    std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
    std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
    
    button->setTextureRect(sf::IntRect(0, 0, 480, 20));
    hover->setTextureRect(sf::IntRect(0, 20, 480, 20));
    click->setTextureRect(sf::IntRect(0, 40, 480, 20));
    
    _items[it.id] = ListItem({_pos.width, _pos.height}, name, player, Button({ 0, 0 , _pos.width, 50 }, button, hover, click));
    _displayRoom.push_back(it);
  }

  for (auto it : toRemove)
  {
    auto end = _displayRoom.end();
    for (auto it1 = _displayRoom.begin(); it1 != end;it1++)
      if (it.id == it1->id)
        end = _displayRoom.erase(it1);
  }
}