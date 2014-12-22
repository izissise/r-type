#ifndef _MESSAGEBOX_HPP_
# define _MESSAGEBOX_HPP_

# include "ADrawable.hpp"
# include "Text.hpp"
# include "Room.hpp"
# include "RessourceManager.hpp"

class MessageBox: public ADrawable
{
public:
  MessageBox(const sf::FloatRect &pos, std::vector<std::string> &vec, bool focus = false);
  ~MessageBox();
  
  virtual void  update(const sf::Event &, float timeElapsed);
  virtual void  draw(sf::RenderWindow &);
  
private:
  void  updateEntry();
  bool  isSame() const;
  
  std::vector<Text>               _text;
  std::vector<std::string>        &_vec;
  bool                            _focus;
  float                           _cam;
};

#endif