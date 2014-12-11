#include "ClientGame.hpp"

ClientGame::ClientGame()
: _win({1920, 1080}, "R-Type"), _done(false)
{
  createMenuPanel();
//  _panel.push_back(std::shared_ptr<APanel>(new ConnectionPanel(true)));
}

ClientGame::~ClientGame()
{
  
}

void ClientGame::run()
{
  while(!_done)
  {
    auto t_start = std::chrono::high_resolution_clock::now();
    double fps = 1000 / 60;

    if (!update())
      _done = true;
    draw();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t = t_end - t_start;
    if (t.count() < fps)
      std::this_thread::sleep_for( std::chrono::milliseconds( static_cast<int>(fps - t.count())) );
  }
}

bool  ClientGame::update()
{
  sf::Event event;

  for (auto it : _panel)
    if (!it->isHidden())
      while (_win.pollEvent(event))
      {
        if (event.type == sf::Event::Closed)
          return (false);
        it->update(event);
      }
  return (true);
}

void  ClientGame::draw()
{
  _win.clear();
  for (auto it : _panel)
    if (!it->isHidden())
    {
      it->draw(_win);
      break;
    }
  _win.display();
}

void	ClientGame::trigger(const t_event &event)
{
  std::cout << "TRIGGER" << std::endl;
}

void  ClientGame::createMenuPanel()
{
  auto menuPanel = std::shared_ptr<Panel>(new Panel());
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/menuBackground.png");
  auto texture = RessourceManager::instance().getTexture("../assets/menu.png");
  auto font = RessourceManager::instance().getFont("../assets/font.ttf");
  
  std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  background(new sf::Sprite(*backgroundTexture));
  
  button->setTextureRect(sf::IntRect(0, 0, 100, 50));
  hover->setTextureRect(sf::IntRect(0, 50, 100, 50));
  click->setTextureRect(sf::IntRect(0, 100, 100, 50));
  
  auto _ipEntry = std::shared_ptr<TextEntry>(new TextEntry("ip:port", {0, 50, 200, 50}, button));
  _ipEntry->setFont(*font);
  _ipEntry->setTextColor(sf::Color::White);
  _ipEntry->setCharacterSize(30);
  
  auto _loginEntry = std::shared_ptr<TextEntry>(new TextEntry("login", {0, 0, 100, 50}, button));
  _loginEntry->setFont(*font);
  _loginEntry->setTextColor(sf::Color::White);
  _loginEntry->setCharacterSize(30);
  
  auto _connect = std::shared_ptr<Button>(new Button({ 100, 100 , 200, 50}, button, hover, click));
  auto _setting = std::shared_ptr<Button>(new Button({ 100, 150 , 100, 50 }, button, hover, click));
  auto _exit = std::shared_ptr<Button>(new Button({ 100, 200 , 50, 50 }, button, hover, click));
  auto _background = std::shared_ptr<Image>(new Image(background));
  
  _exit->onClick([this]() { _done = true; });
  
  menuPanel->add(_background);
  menuPanel->add(_ipEntry);
  menuPanel->add(_loginEntry);
  menuPanel->add(_connect);
  menuPanel->add(_setting);
  menuPanel->add(_exit);
  _panel.push_back(menuPanel);
}