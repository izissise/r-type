#include "ClientGame.hpp"

ClientGame::ClientGame()
: _win({800, 600}, "R-Type"), _done(false)
{
  _panel.push_back(std::shared_ptr<APanel>(new MenuPanel(false)));
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
        if (event.type == sf::Event::Closed || !it->update(event))
          return (false);
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
