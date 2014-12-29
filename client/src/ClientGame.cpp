#include "ClientGame.hpp"

template<>
std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> RtypeProtoHelper<ClientGame>::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientGame::netShortResponse},
  {Packet::APacket::PacketType::GETLISTROOM, &ClientGame::netGetListRoom},
  {Packet::APacket::PacketType::GETLISTPLAYER, &ClientGame::netGetListPlayer},
  {Packet::APacket::PacketType::STARTGAME, &ClientGame::netStartGame},
  {Packet::APacket::PacketType::MESSAGE, &ClientGame::netMessage},
};

ClientGame::ClientGame()
: _ip(""), _win({1600, 900}, "R-Type"), _done(false), _isLoading(false),
_network(Network::NetworkFactory::createNetwork()), _login(""), _game(new Game(sf::FloatRect( 0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y) ))),
_input()
{
  sf::Image icon;

  if (icon.loadFromFile("../assets/icon.png"))
    _win.setIcon(icon.getSize().x , icon.getSize().y, icon.getPixelsPtr());
  if (_music.openFromFile("../assets/menu.ogg"))
  {
    _music.play();
    _music.setLoop(true);
  }
  _input.setAction("fire", sf::Keyboard::Space);
  _input.setAction("up", sf::Keyboard::Up);
  _input.setAction("down", sf::Keyboard::Down);
  _input.setAction("left", sf::Keyboard::Left);
  _input.setAction("right", sf::Keyboard::Right);
  _currentPanel = Panel::PanelId::MENUPANEL;
  createMenuPanel();
  createListPanel();
  createCreateRoomPanel();
  createRoomPanel();
  _panel[Panel::PanelId::GAMEPANEL] = _game;
}

ClientGame::~ClientGame()
{

}

void ClientGame::run()
{
  std::chrono::duration<double, std::milli> t(0);
  while(!_done)
  {
    auto t_start = std::chrono::steady_clock::now();
   // double fps = 1000 / 60;

    if (!update(t.count()))
      _done = true;
    draw();
	t = std::chrono::steady_clock::now() - t_start;
  }
}

void ClientGame::onDisconnet()
{
  std::cout << "Disconnected" << std::endl;
  _currentPanel = Panel::PanelId::MENUPANEL;
  _list.clear();
  _player.clear();
  _chat.clear();
  _isLoading = false;
}

bool  ClientGame::update(float timeElapsed)
{
  sf::Event event;

  if (isConnected())
    _network->poll();
  _input.update(event);
  while (_win.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      return (false);
    _input.update(event);
  }
  if (!_isLoading)
    _panel[_currentPanel]->update(_input, timeElapsed);
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
    _isLoading = false;
    switch (_currentPanel)
    {
      case Panel::PanelId::MENUPANEL:
      {
        Packet::AskListRoom ask;
        _currentPanel = Panel::PanelId::LISTPANEL;
        _writeBuff.writeBuffer(ask.to_bytes());
        break;
      }
      case Panel::PanelId::CREATEROOMPANEL:
      case Panel::PanelId::LISTPANEL:
      {
        _currentPanel = Panel::PanelId::ROOMPANEL;
        break;
      }
      default:
        break;
    }
  }
  else if (_isLoading)
  {
    _isLoading = false;
    if (_currentPanel == Panel::PanelId::MENUPANEL)
      _socket.reset();
    std::cerr << "Abort" << std::endl;
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

  nbUsed = cr.from_bytes(data);
  _list.clear();
  _list = cr.getListRoom();
  std::cout << "Get Room = [";
  for (auto& it : _list)
  {
    std::cout << it.name << ",";
  }
  std::cout << "]" << std::endl;
  return nbUsed;
}

size_t  ClientGame::netGetListPlayer(const Network::Buffer &data)
{
  Packet::GetListPlayer cr;
  size_t  nbUsed;

  nbUsed = cr.from_bytes(data);
  _player.clear();
  auto tmpPlayers = cr.getPlayerList();
  std::cout << "Get Player = [";
  for (auto& it : tmpPlayers)
  {
  	_player.push_back(it.name);
    std::cout << it.name << ",";
  }
  std::cout << "]" << std::endl;
  return nbUsed;
}

size_t  ClientGame::netStartGame(const Network::Buffer &data)
{
  Packet::StartGame rep;
  size_t  nbUsed;
  std::stringstream ss("");

  nbUsed = rep.from_bytes(data);
  std::cout << "Ip = " << _ip << " | Port = " << rep.getPort() << " | Id = " << rep.getPlayerId() << std::endl;
  ss << rep.getPort();
  if (_game->connect(_ip, ss.str(), _login, rep.getPlayerId()))
    _currentPanel = Panel::PanelId::GAMEPANEL;
  if (_music.getStatus() == sf::SoundSource::Playing)
    _music.stop();
  return nbUsed;
}

size_t  ClientGame::netMessage(const Network::Buffer &data)
{
  Packet::Message rep;
  size_t  nbUsed;

  nbUsed = rep.from_bytes(data);
  _chat.push_back(rep.getMsg());
  std::cout << "Message = " << rep.getMsg() << std::endl;
  return nbUsed;
}

void  ClientGame::createMenuPanel()
{
	std::shared_ptr<Panel> menuPanel(new Panel(sf::FloatRect( 0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y) )));
  auto backgroundTexture = RessourceManager::instance().getTexture("../assets/connectionBackground.png");
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

  std::function<void ()> func([this, loginEntry, ipEntry]() {
    std::string login = loginEntry->getText();
    std::string ip = ipEntry->getText();

    if (!login.empty() && !ip.empty())
    {
      size_t nbColon = std::count(ip.begin(), ip.end(), ':');
      std::string realIp = (nbColon % 2 == 0 ? ip : ip.substr(0, ip.find_last_of(':')));
      std::string realPort = (nbColon % 2 == 1 ? ip.substr(ip.find_last_of(':') + 1) : DEFAULTPORT);
      try {
        std::shared_ptr<Network::ABasicSocket> socket = Network::NetworkFactory::createConnectSocket(realIp, realPort);
        Packet::Handshake packet(login);
        _network->registerClient(socket);
        setSocket(socket);
        _writeBuff.writeBuffer(packet.to_bytes());
        _isLoading = true;
        _login = login;
        _list.clear();
        _ip = ip;
      }
      catch (Network::Error &e) {
        std::cerr << e.what() << std::endl;
      }
    }
    else
      std::cerr << "The login or the ip is not fill" << std::endl;
  });

  exit->onClick([this]() { _done = true; });
  connect->onClick(func);

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
	std::shared_ptr<Panel> listpanel(new Panel(sf::FloatRect(0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)
)));

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
    _isLoading = false;
    _currentPanel = Panel::PanelId::CREATEROOMPANEL;
  });
  disconnect->onClick([this]() {
    _isLoading = false;
    _list.clear();
    _socket.reset();
    _currentPanel = Panel::PanelId::MENUPANEL;
  });

  listpanel->add(back);
  listpanel->add(create);
  listpanel->add(disconnect);
  listpanel->add(std::shared_ptr<ADrawable>(new ListBox({0, 0, 1300, 900}, _list,
                                                        [this](const uint32_t id) {
                                                          _isLoading = true;
                                                          Packet::JoinRoom join(id);
                                                          _writeBuff.writeBuffer(join.to_bytes());
                                                        } )));
  _panel[Panel::PanelId::LISTPANEL] = listpanel;
}

void  ClientGame::createCreateRoomPanel()
{
	std::shared_ptr<Panel>  panel(new Panel(sf::FloatRect(0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)
)));

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

  std::shared_ptr<Text> enterText(new Text({(static_cast<float>(_win.getSize().x) / 2) - 400,
    (static_cast<float>(_win.getSize().y) / 2), 300, 60}, "Room's Name:"));
  std::shared_ptr<TextEntry> entry(new TextEntry("", {(static_cast<float>(_win.getSize().x) / 2) - 100 ,
    (static_cast<float>(_win.getSize().y) / 2), 400, 60}, button));

  entry->setFont(*font);
  entry->setTextColor(sf::Color::White);
  entry->setCharacterSize(30);

  enterText->setFont(*font);
  enterText->setColor(sf::Color::White);
  enterText->setCharacterSize(30);

  std::shared_ptr<Button> create(new Button({ 1300, 745 , 300, 75 }, button, hover, click, cr));
  std::shared_ptr<Button> cancel(new Button({ 1300, 825 , 300, 75 }, button, hover, click, d));
  std::shared_ptr<Image> back(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));

  std::function<void ()> func([this, entry]() {
    if (_isLoading)
      return ;
    if (entry->getText().empty())
    {
      std::cerr << "The room's name cannot be empty" << std::endl;
      return ;
    }
    Packet::CreateRoom room({entry->getText(), 0, 4, 0});

    _isLoading = true;
    entry->setText("");
    _writeBuff.writeBuffer(room.to_bytes());
  });
;

  create->onClick(func);
  cancel->onClick([this]() {
    _isLoading = false;
    _currentPanel = Panel::PanelId::LISTPANEL;
  });

  panel->add(back);
  panel->add(enterText);
  panel->add(entry);
  panel->add(create);
  panel->add(cancel);
  _panel[Panel::PanelId::CREATEROOMPANEL] = panel;
}

void  ClientGame::createRoomPanel()
{
  std::shared_ptr<Panel> panel(new Panel(sf::FloatRect(0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y))));

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


  std::shared_ptr<Text> ready(new Text({0, 0, 0, 0}, "Ready"));
  std::shared_ptr<Text> send(new Text({0, 0, 0, 0}, "Send"));
  std::shared_ptr<Text> d(new Text({0, 0, 0, 0}, "Leave Room"));

  ready->setFont(*font);
  ready->setColor(sf::Color::White);
  ready->setCharacterSize(30);

  send->setFont(*font);
  send->setColor(sf::Color::White);
  send->setCharacterSize(30);

  d->setFont(*font);
  d->setColor(sf::Color::White);
  d->setCharacterSize(30);

  std::shared_ptr<Button> r(new Button({ 1300, 745 , 300, 75 }, button, hover, click, ready));
  std::shared_ptr<Button> sendButton(new Button({ 1000, 825 , 300, 75 }, button, hover, click, send));
  std::shared_ptr<Button> disconnect(new Button({ 1300, 825 , 300, 75 }, button, hover, click, d));
  std::shared_ptr<Image> back(new Image(background, {0, 0, static_cast<float>(_win.getSize().x), static_cast<float>(_win.getSize().y)}));
  std::shared_ptr<TextEntry> entry(new TextEntry("", { 0, static_cast<float>(_win.getSize().y) - 60,
                                                      static_cast<float>(_win.getSize().x) - sendButton->getSize().x - disconnect->getSize().x, 60}, button));

  r->onClick([this]() {
    Network::Buffer msg;
    Packet::APacket::fill_bytes(msg, Packet::APacket::fromPacketType(Packet::APacket::PacketType::READYGAME));
    _writeBuff.writeBuffer(msg);
  });

  sendButton->onClick([this, entry]() {
      _writeBuff.writeBuffer(Packet::Message(entry->getText()));
      entry->setText("");
  });

  disconnect->onClick([this, entry]() {
    entry->setText("");
    Network::Buffer msg;
    Packet::APacket::fill_bytes(msg, Packet::APacket::fromPacketType(Packet::APacket::PacketType::LEAVEROOM));
    _writeBuff.writeBuffer(msg);
    _chat.clear();
    _player.clear();
    _currentPanel = Panel::PanelId::LISTPANEL;
  });

  entry->setFont(*font);
  entry->setTextColor(sf::Color::White);
  entry->setCharacterSize(30);

  std::shared_ptr<GameMessageBox> _chatBox(new GameMessageBox({0, 0, 1299, static_cast<float>(_win.getSize().y) - 61}, _chat, true));
  std::shared_ptr<GameMessageBox> _playerBox(new GameMessageBox({1300, 0 , 300, 745}, _player));

  panel->add(back);
  panel->add(entry);
  panel->add(_chatBox);
  panel->add(_playerBox);
  panel->add(sendButton);
  panel->add(r);
  panel->add(disconnect);

  _panel[Panel::PanelId::ROOMPANEL] = panel;
}
