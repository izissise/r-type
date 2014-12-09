#include "MenuPanel.hpp"

MenuPanel::MenuPanel(bool hide)
: APanel(hide), _background()
{
  std::shared_ptr<sf::Texture> text(new sf::Texture());
  
  if (!text->loadFromFile("../assets/menu.png"))
    throw std::runtime_error("Cannot load the Texture file.");

  sf::Sprite  *button = new sf::Sprite(*text);
  sf::Sprite  *hover = new sf::Sprite(*text);
  sf::Sprite  *click = new sf::Sprite(*text);

  button->setTextureRect(sf::IntRect(0, 0, 100, 50));
  hover->setTextureRect(sf::IntRect(0, 50, 100, 50));
  click->setTextureRect(sf::IntRect(0, 100, 100, 50));
  _connect = std::shared_ptr<Button>(new Button({ 100, 100 }, std::shared_ptr<sf::Sprite>(button),
                                                std::shared_ptr<sf::Sprite>(hover), std::shared_ptr<sf::Sprite>(click)));
//  _setting = std::shared_ptr(new Button({ 100, 200 }, ));
//  _exit = std::shared_ptr(new Button({ 100, 300 }, ));
  _text.push_back(text);

  std::shared_ptr<sf::Texture> t(new sf::Texture());

  if (!t->loadFromFile("../assets/menuBackground.png"))
    throw std::runtime_error("Cannot load the Texture file.");
  _background.setTexture(*t);
  _text.push_back(t);
}

MenuPanel::~MenuPanel()
{
  
}

bool MenuPanel::update(const sf::Event &event)
{
  _connect->update(event);
  if (_connect->isClicked())
    std::cout << "Connect" << std::endl;
//  _setting->update(event);
//  _exit->update(event);
//  if (_exit->isClicked())
//    return (false);
  return (true);
}

void MenuPanel::draw(sf::RenderWindow &win)
{
  win.draw(_background);
  _connect->draw(win);
//  _setting->draw(win);
//  _exit->draw(win);
}
