#include "ClientGame.hpp"

std::map<Packet::APacket::PacketType, bool (ClientGame::*)(const Network::Buffer&)> ClientGame::_netWorkBinds =
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

void  ClientGame::onRead(size_t sizeRead)
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;
  bool isPacket = false;
  
  if (sizeRead == 0)
    return;
  
  while (!isPacket && _readBuff.getLeftRead() >= headerSize)
  {
    _readBuff.readBuffer(buff, headerSize);
    pack = Packet::APacket::toPacketType(buff);
    if (pack != Packet::APacket::PacketType::UNKNOW)
    {
      isPacket = true;
      bool ret = (this->*_netWorkBinds.at(pack))(buff);
      if (!ret)
        _readBuff.rollbackReadBuffer(headerSize);
    }
    else
    {
      _readBuff.rollbackReadBuffer(headerSize - 1);
      std::cout << "Unknown Packet" << std::endl;
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
    _writeBuff.writeBuffer(ask.to_bytes());
  }
}

bool  ClientGame::netShortResponse(const Network::Buffer& data)
{
  size_t readSize = 200;
  Packet::ShortResponse rep;
  size_t  nbUsed;
  
  try {
    Network::Buffer buff;
    Network::Buffer tmpBuff(data);
    _readBuff.readBuffer(buff, readSize);
    readSize = buff.size();
    tmpBuff += buff;
    nbUsed = rep.from_bytes(tmpBuff);
    _readBuff.rollbackReadBuffer(readSize - nbUsed);
    std::cout << "Get Response = " << static_cast<int>(rep.getResponse()) << std::endl;
    checkReponse(rep.getResponse());
  }
  catch (std::exception& e)
  {
    _readBuff.rollbackReadBuffer(readSize);
    return false;
  }
  return true;
}

bool  ClientGame::netGetListRoom(const Network::Buffer& data)
{
  size_t readSize = 200;
  Packet::GetListRoom cr;
  size_t  nbUsed;
  
  try {
    Network::Buffer buff;
    Network::Buffer tmpBuff(data);
    _readBuff.readBuffer(buff, readSize);
    readSize = buff.size();
    tmpBuff += buff;
    nbUsed = cr.from_bytes(tmpBuff);
    _readBuff.rollbackReadBuffer(readSize - nbUsed);
    std::cout << "GetListRoom" << std::endl;
    for (auto it : cr.getListRoom())
      std::cout << "Get Room = [" << it.name << "]" << std::endl;
  }
  catch (std::exception& e)
  {
    _readBuff.rollbackReadBuffer(readSize);
    return false;
  }
  return true;
}

void  ClientGame::createMenuPanel()
{
  auto menuPanel = std::shared_ptr<Panel>(new Panel({0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
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

  
  auto loginEntry = std::shared_ptr<TextEntry>(new TextEntry("login", {(static_cast<float>(_win.getSize().x) / 2) - 200, 600, 400, 60}, button));

  loginEntry->setFont(*font);
  loginEntry->setTextColor(sf::Color::White);
  loginEntry->setCharacterSize(30);

  auto ipEntry = std::shared_ptr<TextEntry>(new TextEntry("ip:port", {(static_cast<float>(_win.getSize().x) / 2) - 200, 680, 400, 60}, button));
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

  
  auto connect = std::shared_ptr<Button>(new Button({ 1350, 665 , 250, 75 }, button, hover, click, connectText));
  auto setting = std::shared_ptr<Button>(new Button({ 1350, 745 , 250, 75 }, button, hover, click, settingsText));
  auto exit = std::shared_ptr<Button>(new Button({ 1350, 825 , 250, 75 }, button, hover, click, quitText));
  auto back = std::shared_ptr<Image>(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  exit->onClick([this]() { _done = true; });
  connect->onClick([this, loginEntry, ipEntry](){
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
      } catch (Network::Error &e) {
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
  auto listpanel = std::shared_ptr<Panel>(new Panel({0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
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

  auto create = std::shared_ptr<Button>(new Button({ 1300, 745 , 300, 75 }, button, hover, click, cr));
  auto disconnect = std::shared_ptr<Button>(new Button({ 1300, 825 , 300, 75 }, button, hover, click, d));
  auto back = std::shared_ptr<Image>(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  
  create->onClick([this]() {
    Packet::CreateRoom room({"toto", 0, 4, 0});
    
    std::cout << "Create Room" << std::endl;
    _socket->setEventRequest(Network::ASocket::Event::RDWR);
    _writeBuff.writeBuffer(room.to_bytes());
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
