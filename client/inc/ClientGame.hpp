#ifndef _CLIENTGAME_HPP_
# define _CLIENTGAME_HPP_

# include <SFML/Graphics.hpp>
# include <algorithm>
# include <iostream>
# include <thread>
# include <memory>
# include "Observer.hpp"
# include "Panel.hpp"
# include "TextEntry.hpp"
# include "Button.hpp"
# include "Image.hpp"
# include "RessourceManager.hpp"
# include "NetworkFactory.hpp"
# include "Handshake.hpp"
# include "ClientHelper.hpp"
# include "ListPanel.hpp"
# include "Room.hpp"

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

  static std::map<Packet::APacket::PacketType, bool (ClientGame::*)(const Network::Buffer&)> _netWorkBinds;

  bool netShortResponse(const Network::Buffer& data);
  bool netGetListRoom(const Network::Buffer& data);
  bool netCreateRoom(const Network::Buffer& data);
  bool netJoinRoom(const Network::Buffer& data);
  
  void  createMenuPanel();
  void  createListPanel();

  sf::RenderWindow                                        _win;
  bool                                                    _done;
  Panel::PanelId                                          _currentPanel;
  std::map<Panel::PanelId, std::shared_ptr<Panel>>        _panel;
  std::unique_ptr<Network::ANetwork>                      _network;
  std::vector<t_room>                                     _list;
};

#endif