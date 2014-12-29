#ifndef _CLIENTGAME_HPP_
# define _CLIENTGAME_HPP_

# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>
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
# include "RtypeProtoHelper.hpp"
# include "ListBox.hpp"
# include "Room.hpp"
# include "GameMessageBox.hpp"
# include "Game.hpp"
# include "Input.hpp"

# include "Packet/Handshake.hpp"
# include "Packet/ShortResponse.hpp"
# include "Packet/GetListRoom.hpp"
# include "Packet/AskListRoom.hpp"
# include "Packet/CreateRoom.hpp"
# include "Packet/JoinRoom.hpp"
# include "Packet/GetListPlayer.hpp"
# include "Packet/StartGame.hpp"
# include "Packet/Message.hpp"

# define DEFAULTPORT "8000"

class ClientGame: public RtypeProtoHelper<ClientGame>
{
  friend RtypeProtoHelper<ClientGame>;

public:
  ClientGame();
  ~ClientGame();

  void  run();
  bool  update(float timeElapsed);
  void  draw();

protected:
  void  onDisconnet() override;

private:
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
  std::vector<std::string>    		                      _player;
  std::vector<std::string>                                _chat;
  std::string                                             _login;
  std::shared_ptr<Game>                                   _game;
  sf::Music                                               _music;
  Input                                                   _input;
};

#endif
