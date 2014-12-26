#ifndef _GAMEMESSAGEBOX_HPP_
# define _GAMEMESSAGEBOX_HPP_

# include "ADrawable.hpp"
# include "Text.hpp"
# include "Room.hpp"
# include "RessourceManager.hpp"

class GameMessageBox: public ADrawable
{
public:
  GameMessageBox(const sf::FloatRect &pos, const std::vector<std::string> &vec, bool focus = false);
  ~GameMessageBox();

  virtual void  update(const sf::Event &, float timeElapsed);
  virtual void  draw(sf::RenderWindow &);

private:
  void  updateEntry();
  bool  isSame() const;

  std::vector<Text>               _text;
  const std::vector<std::string>& _vec;
  bool                            _focus;
  float                           _cam;
};

#endif