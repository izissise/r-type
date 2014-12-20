#ifndef _CLIENTGAME_HPP_
# define _CLIENTGAME_HPP_

# include <SFML/Graphics.hpp>
# include <algorithm>
# include <iostream>
# include <thread>
# include <memory>
# include <sstream>
# include "Observer.hpp"
# include "Panel.hpp"
# include "TextEntry.hpp"
# include "Button.hpp"
# include "Image.hpp"
# include "RessourceManager.hpp"
# include "NetworkFactory.hpp"
# include "Handshake.hpp"
# include "ClientHelper.hpp"
# include "ListBox.hpp"
# include "Room.hpp"
# include "MessageBox.hpp"
# include "Game.hpp"

# include "ShortResponse.hpp"
# include "GetListRoom.hpp"
# include "AskListRoom.hpp"
# include "CreateRoom.hpp"
# include "JoinRoom.hpp"
# include "GetListPlayer.hpp"
# include "StartGame.hpp"
# include "Message.hpp"

# define DEFAULTPORT "8000"

class ClientGame: public Network::SocketClientHelper
{
public:
  ClientGame();
  ~ClientGame();

  void  run();
  bool  update();
  void  draw();


private:
  void  onRead(size_t sizeRead) override;
  void  onWrite(size_t sizeWrite) override;
  void  onDisconnet() override;

  static std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> _netWorkBinds;

  size_t  netShortResponse(const Network::Buffer& data);
  size_t  netGetListRoom(const Network::Buffer& data);
  size_t  netGetListPlayer(const Network::Buffer &data);
  size_t  netStartGame(const Network::Buffer &data);
  size_t  netMessage(const Network::Buffer &data);

  void  checkReponse(uint8_t rep);

  void  createMenuPanel();
  void  createListPanel();
  void  createCreateRoomPanel();
  void  createRoomPanel();
  void  createGamePanel();

  sf::RenderWindow                                        _win;
  bool                                                    _done;
  bool                                                    _isLoading;
  Panel::PanelId                                          _currentPanel;
  std::map<Panel::PanelId, std::shared_ptr<Panel>>        _panel;
  std::unique_ptr<Network::ANetwork>                      _network;
  std::vector<t_room>                                     _list;
  std::vector<Packet::PlayerClient>                       _player;
  std::vector<std::string>                                _chat;
  std::string                                             _login;
  std::shared_ptr<Game>                                   _game;
};

#endif