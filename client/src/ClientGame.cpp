#include "ClientGame.hpp"

std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> ClientGame::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientGame::netShortResponse},
  {Packet::APacket::PacketType::GETLISTROOM, &ClientGame::netGetListRoom},
};

ClientGame::ClientGame()
: SocketClientHelper(), _win({1600, 900}, "R-Type"), _done(false), _isLoading(false), _network(Network::NetworkFactory::createNetwork())
{
  _currentPanel = Panel::PanelId::MENUPANEL;
  createMenuPanel();
  createListPanel();
  createCreateRoomPanel();
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

void  ClientGame::onRead(size_t nbRead)
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;
  bool isPacket = false;

  if (nbRead == 0)
    return;
  while (!isPacket && _readBuff.getLeftRead() >= headerSize)
    {
      _readBuff.readBuffer(buff, headerSize);
      pack = Packet::APacket::toPacketType(buff);
      if (pack != Packet::APacket::PacketType::UNKNOW)
        {
          buff.clear();
          _readBuff.readBuffer(buff, _readBuff.getLeftRead());
          isPacket = true;
          try {
              size_t (ClientGame::*meth)(const Network::Buffer&) = _netWorkBinds.at(pack);
              try {
                  size_t nbUsed = (this->*meth)(buff);
                  _readBuff.rollbackReadBuffer(buff.size() - nbUsed);
                }
              catch (std::exception& e)
                {
                  _readBuff.rollbackReadBuffer(buff.size() - 1);
                }
            }
          catch (std::exception& e)
            {
              _readBuff.rollbackReadBuffer(headerSize - 1);
            }
        }
      else
        {
          _readBuff.rollbackReadBuffer(headerSize - 1);
          std::cerr << "Received Unknown Packet" << std::endl;
        }
    }
}

void  ClientGame::onWrite(size_t sizeWrite)
{
  std::cout << "Write" << std::endl;
}

void ClientGame::onDisconnet()
{
  std::cout << "Disconnected" << std::endl;
  _currentPanel = Panel::PanelId::MENUPANEL;
}

bool  ClientGame::update()
{
  sf::Event event;

  if (isConnected())
    _network->poll();
  while (_win.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        return (false);
      _panel[_currentPanel]->update(event);
    }
  return (true);
}

void  ClientGame::draw()
{
  _win.clear();
  _panel[_currentPanel]->draw(_win);
  _win.display();
}

void  ClientGame::checkReponse(uint8_t rep)
{
  if (_isLoading && rep == 1)
    {
      Packet::AskListRoom ask;

      _currentPanel = Panel::PanelId::LISTPANEL;
      _isLoading = false;
      _writeBuff.writeBuffer(ask.to_bytes());
    }
}

size_t  ClientGame::netShortResponse(const Network::Buffer& data)
{
  Packet::ShortResponse rep;
  size_t  nbUsed;

  nbUsed = rep.from_bytes(data);
  std::cout << "Get Response = " << static_cast<int>(rep.getResponse()) << std::endl;
  checkReponse(rep.getResponse());
  return nbUsed;
}

size_t  ClientGame::netGetListRoom(const Network::Buffer& data)
{
  Packet::GetListRoom cr;
  size_t  nbUsed;

  std::cout << "GetListRoom" << std::endl;
  nbUsed = cr.from_bytes(data);
  _list.clear();
  _list = cr.getListRoom();
  for (auto it : _list)
    std::cout << "Get Room = [" << it.name << "]" << std::endl;
  return nbUsed;
}

void  ClientGame::createMenuPanel()
{
  std::shared_ptr<Panel> menuPanel(new Panel({0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/menuBackground.png");
  auto texture = RessourceManager::instance().getTexture("../assets/button.png");
  auto font = RessourceManager::instance().getFont("../assets/font.ttf");

  std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  background(new sf::Sprite(*backgroundTexture));

  button->setTextureRect(sf::IntRect(0, 0, 200, 20));
  hover->setTextureRect(sf::IntRect(0, 20, 200, 20));
  click->setTextureRect(sf::IntRect(0, 40, 200, 20));

  std::shared_ptr<TextEntry> loginEntry(new TextEntry("login", {(static_cast<float>(_win.getSize().x) / 2) - 200, 600, 400, 60}, button));

  loginEntry->setFont(*font);
  loginEntry->setTextColor(sf::Color::White);
  loginEntry->setCharacterSize(30);

  std::shared_ptr<TextEntry> ipEntry(new TextEntry("ip:port", {(static_cast<float>(_win.getSize().x) / 2) - 200, 680, 400, 60}, button));
  ipEntry->setFont(*font);
  ipEntry->setTextColor(sf::Color::White);
  ipEntry->setCharacterSize(30);
  
  std::shared_ptr<Text> connectText(new Text({0, 0, 0, 0}, "Connect"));
  std::shared_ptr<Text> settingsText(new Text({0, 0, 0, 0}, "Settings"));
  std::shared_ptr<Text> quitText(new Text({0, 0, 0, 0}, "Quit"));

  connectText->setFont(*font);
  settingsText->setFont(*font);
  quitText->setFont(*font);
  connectText->setColor(sf::Color::White);
  settingsText->setColor(sf::Color::White);
  quitText->setColor(sf::Color::White);
  connectText->setCharacterSize(30);
  settingsText->setCharacterSize(30);
  quitText->setCharacterSize(30);

  std::shared_ptr<Button> connect(new Button({ 1350, 665 , 250, 75 }, button, hover, click, connectText));
  std::shared_ptr<Button> setting(new Button({ 1350, 745 , 250, 75 }, button, hover, click, settingsText));
  std::shared_ptr<Button> exit(new Button({ 1350, 825 , 250, 75 }, button, hover, click, quitText));
  std::shared_ptr<Image> back(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  exit->onClick([this]() { _done = true; });
  connect->onClick([this, loginEntry, ipEntry]() {
    std::string login = loginEntry->getText();
    std::string ip = ipEntry->getText();

    if (!login.empty() && !ip.empty())
      {
        int nbColon = std::count(ip.begin(), ip.end(), ':');
        try {
            std::shared_ptr<Network::ABasicSocket> socket = Network::NetworkFactory::createConnectSocket((nbColon % 2 == 0 ? ip : ip.substr(0, ip.find_last_of(':'))),
                (nbColon % 2 == 1 ? ip.substr(ip.find_last_of(':') + 1) : DEFAULTPORT ));
            Packet::Handshake packet(login);
            _network->registerClient(socket);
            setSocket(socket);
            _writeBuff.writeBuffer(packet.to_bytes());
            socket->setEventRequest(Network::ASocket::Event::RDWR);
            _isLoading = true;
          }
        catch (Network::Error &e) {
            std::cerr << e.what() << std::endl;
          }
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
  _panel[Panel::PanelId::MENUPANEL] = menuPanel;
}

void  ClientGame::createListPanel()
{
  std::shared_ptr<Panel> listpanel(new Panel({0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/menuBackground.png");
  auto texture = RessourceManager::instance().getTexture("../assets/button.png");
  auto font = RessourceManager::instance().getFont("../assets/font.ttf");
  
  std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  background(new sf::Sprite(*backgroundTexture));
  
  button->setTextureRect(sf::IntRect(0, 0, 200, 20));
  hover->setTextureRect(sf::IntRect(0, 20, 200, 20));
  click->setTextureRect(sf::IntRect(0, 40, 200, 20));
  
  std::shared_ptr<Text> cr(new Text({0, 0, 0, 0}, "Create Room"));
  std::shared_ptr<Text> d(new Text({0, 0, 0, 0}, "Disconnect"));
  
  cr->setFont(*font);
  cr->setColor(sf::Color::White);
  cr->setCharacterSize(30);
  
  d->setFont(*font);
  d->setColor(sf::Color::White);
  d->setCharacterSize(30);
  
  std::shared_ptr<Button> create(new Button({ 1300, 745 , 300, 75 }, button, hover, click, cr));
  std::shared_ptr<Button> disconnect(new Button({ 1300, 825 , 300, 75 }, button, hover, click, d));
  std::shared_ptr<Image> back(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  create->onClick([this]() {
    _currentPanel = Panel::PanelId::CREATEROOMPANEL;
  });
  disconnect->onClick([this]() {
    _socket.reset();
    _currentPanel = Panel::PanelId::MENUPANEL;
  });
  
  listpanel->add(back);
  listpanel->add(create);
  listpanel->add(disconnect);
  
  listpanel->add(std::shared_ptr<ADrawable>(new ListBox({0, 0, 1300, 900}, _list)));
  _panel[Panel::PanelId::LISTPANEL] = listpanel;
}

void  ClientGame::createCreateRoomPanel()
{
  std::shared_ptr<Panel>  panel(new Panel({0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/menuBackground.png");
  auto texture = RessourceManager::instance().getTexture("../assets/button.png");
  auto font = RessourceManager::instance().getFont("../assets/font.ttf");
  
  std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
  std::shared_ptr<sf::Sprite>  background(new sf::Sprite(*backgroundTexture));
  
  button->setTextureRect(sf::IntRect(0, 0, 200, 20));
  hover->setTextureRect(sf::IntRect(0, 20, 200, 20));
  click->setTextureRect(sf::IntRect(0, 40, 200, 20));
  
  std::shared_ptr<Text> cr(new Text({0, 0, 0, 0}, "Create Room"));
  std::shared_ptr<Text> d(new Text({0, 0, 0, 0}, "Cancel"));
  
  cr->setFont(*font);
  cr->setColor(sf::Color::White);
  cr->setCharacterSize(30);
  
  d->setFont(*font);
  d->setColor(sf::Color::White);
  d->setCharacterSize(30);

  std::shared_ptr<Text> enterText(new Text({(static_cast<float>(_win.getSize().x) / 2) - 400, 600, 300, 60}, "Room's Name:"));
  std::shared_ptr<TextEntry> entry(new TextEntry("", {(static_cast<float>(_win.getSize().x) / 2) - 100
    , 600, 400, 60}, button));
  
  entry->setFont(*font);
  entry->setTextColor(sf::Color::White);
  entry->setCharacterSize(30);

  enterText->setFont(*font);
  enterText->setColor(sf::Color::White);
  enterText->setCharacterSize(30);

  std::shared_ptr<Button> create(new Button({ 1300, 745 , 300, 75 }, button, hover, click, cr));
  std::shared_ptr<Button> cancel(new Button({ 1300, 825 , 300, 75 }, button, hover, click, d));
  std::shared_ptr<Image> back(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  create->onClick([this, entry]() {
    if (entry->getText().empty())
    {
      std::cerr << "The room's name cannot be empty" << std::endl;
      return ;
    }
    Packet::CreateRoom room({entry->getText(), 0, 4, 0});
    
    std::cout << "Create Room" << std::endl;
    _socket->setEventRequest(Network::ASocket::Event::RDWR);
    _writeBuff.writeBuffer(room.to_bytes());
  });
  cancel->onClick([this]() {
    _currentPanel = Panel::PanelId::LISTPANEL;
  });

  panel->add(back);
  panel->add(enterText);
  panel->add(entry);
  panel->add(create);
  panel->add(cancel);
  _panel[Panel::PanelId::CREATEROOMPANEL] = panel;
}