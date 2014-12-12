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
  
  button->setTextureRect(sf::IntRect(0, 0, 200, 20));
  hover->setTextureRect(sf::IntRect(0, 20, 200, 20));
  click->setTextureRect(sf::IntRect(0, 40, 200, 20));
  
  auto ipEntry = std::shared_ptr<TextEntry>(new TextEntry("ip:port", {0, 50, 200, 50}, button));
  ipEntry->setFont(*font);
  ipEntry->setTextColor(sf::Color::White);
  ipEntry->setCharacterSize(30);
  
  auto loginEntry = std::shared_ptr<TextEntry>(new TextEntry("login", {0, 0, 100, 50}, button));
  loginEntry->setFont(*font);
  loginEntry->setTextColor(sf::Color::White);
  loginEntry->setCharacterSize(30);
  
  std::shared_ptr<Text> connectText(new Text("Connect"));
  std::shared_ptr<Text> settingsText(new Text("Settings"));
  std::shared_ptr<Text> quitText(new Text("Quit"));
  connectText->setFont(*font);
  settingsText->setFont(*font);
  quitText->setFont(*font);
  connectText->setColor(sf::Color::White);
  settingsText->setColor(sf::Color::White);
  quitText->setColor(sf::Color::White);
  connectText->setCharacterSize(30);
  settingsText->setCharacterSize(30);
  quitText->setCharacterSize(30);
  
  auto connect = std::shared_ptr<Button>(new Button({ 100, 100 , 200, 50}, button, hover, click, connectText));
  auto setting = std::shared_ptr<Button>(new Button({ 100, 150 , 100, 50 }, button, hover, click, settingsText));
  auto exit = std::shared_ptr<Button>(new Button({ 100, 200 , 50, 50 }, button, hover, click, quitText));
  auto back = std::shared_ptr<Image>(new Image(background));
  
  exit->onClick([this]() { _done = true; });
  connect->onClick([this, loginEntry, ipEntry](){
    std::string login = loginEntry->getText();
    std::string ip = ipEntry->getText();

    if (!login.empty() && !ip.empty())
    {
      int nbColon = std::count(ip.begin(), ip.end(), ':');
      _socket = Network::NetworkFactory::createConnectSocket((nbColon % 2 == 0 ? ip : ip.substr(0, ip.find_last_of(':'))),
                                                             (nbColon % 2 == 1 ? ip.substr(ip.find_last_of(':') + 1) : DEFAULTPORT ));
      _socket->write("Salut");
    }
    else
      std::cerr << "The login or the ip is not fill" << std::endl;
  });
  
  menuPanel->add(back);
  menuPanel->add(ipEntry);
  menuPanel->add(loginEntry);
  menuPanel->add(connect);
  menuPanel->add(setting);
  menuPanel->add(exit);
  _panel.push_back(menuPanel);
}