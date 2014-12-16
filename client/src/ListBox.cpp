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
  {
    std::cout << "Differ" << std::endl;
    updateEntry();
  }
  for (auto it : _items)
    it.second->update(event);
}

void  ListBox::draw(sf::RenderWindow &win)
{
  for (auto it : _items)
    it.second->draw(win);
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
    std::shared_ptr<Text>  name(new Text(it.name));
    std::shared_ptr<Text>  player(new Text(ss.str()));
    
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
    
    _items[it.id] = std::shared_ptr<ListItem>(new ListItem({_pos.width, _pos.height},
                                                           name, player,
                                                           std::shared_ptr<Button>(new Button({ 0, 0 , _pos.width, 50 }, button, hover, click))));
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