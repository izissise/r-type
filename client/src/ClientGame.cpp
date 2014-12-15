#include "ClientGame.hpp"

std::map<Packet::APacket::PacketType, bool (ClientGame::*)(const Network::Buffer&)> ClientGame::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientGame::netShortResponse},
  {Packet::APacket::PacketType::GETLISTROOM, &ClientGame::netGetListRoom},
};

ClientGame::ClientGame()
: SocketClientHelper(), _win({1920, 1080}, "R-Type"), _done(false), _network(Network::NetworkFactory::createNetwork())
{
  _currentPanel = Panel::PanelId::MENUPANEL;
  createMenuPanel();
  _panel[Panel::PanelId::LISTPANEL] = std::shared_ptr<Panel>(new ListPanel(_list));
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

bool ClientGame::netShortResponse(const Network::Buffer& data)
{
  size_t readSize = 200;
  Packet::ShortResponse cr;
  size_t  nbUsed;
  
  try {
    Network::Buffer buff;
    Network::Buffer tmpBuff(data);
    _readBuff.readBuffer(buff, readSize);
    readSize = buff.size();
    tmpBuff += buff;
    nbUsed = cr.from_bytes(tmpBuff);
    _readBuff.rollbackReadBuffer(readSize - nbUsed);
    std::cout << "Get Response = " << static_cast<int>(cr.getResponse()) << std::endl;
    return true;
  }
  catch (std::exception& e)
  {
    _readBuff.rollbackReadBuffer(readSize);
    return false;
  }
}

bool ClientGame::netGetListRoom(const Network::Buffer& data)
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
    for (auto it : cr.getListRoom())
      std::cout << "Get Room = [" << it.name << "]" << std::endl;
    return true;
  }
  catch (std::exception& e)
  {
    _readBuff.rollbackReadBuffer(readSize);
    return false;
  }
}

void  ClientGame::createMenuPanel()
{
  auto menuPanel = std::shared_ptr<Panel>(new Panel());
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

  
  auto loginEntry = std::shared_ptr<TextEntry>(new TextEntry("login", {(static_cast<float>(_win.getSize().x) / 2) - 200, 700, 400, 60}, button));

  loginEntry->setFont(*font);
  loginEntry->setTextColor(sf::Color::White);
  loginEntry->setCharacterSize(30);

  auto ipEntry = std::shared_ptr<TextEntry>(new TextEntry("ip:port", {(static_cast<float>(_win.getSize().x) / 2) - 200, 780, 400, 60}, button));
  ipEntry->setFont(*font);
  ipEntry->setTextColor(sf::Color::White);
  ipEntry->setCharacterSize(30);
  
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

  
  auto connect = std::shared_ptr<Button>(new Button({ 1670, 800 , 250, 75 }, button, hover, click, connectText));
  auto setting = std::shared_ptr<Button>(new Button({ 1670, 880 , 250, 75 }, button, hover, click, settingsText));
  auto exit = std::shared_ptr<Button>(new Button({ 1670, 960 , 250, 75 }, button, hover, click, quitText));
  auto back = std::shared_ptr<Image>(new Image(background));
  
  back->setScale(static_cast<float>(_win.getSize().x) / background->getTextureRect().width,
                 static_cast<float>(_win.getSize().y) / background->getTextureRect().height);

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
        _currentPanel = Panel::PanelId::LISTPANEL;
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
