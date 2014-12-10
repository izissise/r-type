#include "MenuPanel.hpp"

MenuPanel::MenuPanel(bool hide)
: APanel(hide), _background()
{
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/menuBackground.png");
  auto texture = RessourceManager::instance().getTexture("../assets/menu.png");
  auto font = RessourceManager::instance().getFont("../assets/font.ttf");
  
  std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));

  button->setTextureRect(sf::IntRect(0, 0, 100, 50));
  hover->setTextureRect(sf::IntRect(0, 50, 100, 50));
  click->setTextureRect(sf::IntRect(0, 100, 100, 50));

  _loginEntry = std::shared_ptr<TextEntry>(new TextEntry("login", {0, 0, 500, 50}, button));
  _loginEntry->setFont(*font);
  _loginEntry->setTextColor(sf::Color::White);
  _loginEntry->setCharacterSize(30);

  _connect = std::shared_ptr<Button>(new Button({ 100, 100 , 200, 50}, button, hover, click));
  _setting = std::shared_ptr<Button>(new Button({ 100, 150 , 100, 50 }, button, hover, click));
  _exit = std::shared_ptr<Button>(new Button({ 100, 200 , 50, 50 }, button, hover, click));

  _background.setTexture(*backgroundTexture);
}

MenuPanel::~MenuPanel()
{
  
}

bool MenuPanel::update(const sf::Event &event)
{
  _loginEntry->update(event);
  _connect->update(event);
  if (_connect->isClicked())
    std::cout << "Connect" << std::endl;
  _setting->update(event);
  _exit->update(event);
  if (_exit->isClicked())
    return (false);
  return (true);
}

void MenuPanel::draw(sf::RenderWindow &win)
{
  win.draw(_background);
  _loginEntry->draw(win);
  _connect->draw(win);
  _setting->draw(win);
  _exit->draw(win);
}
