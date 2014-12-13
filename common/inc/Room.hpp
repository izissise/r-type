#ifndef _ROOM_HPP_
# define _ROOM_HPP_

# include <string>

typedef struct  s_room
{
  std::string   name;
  uint32_t      id;
  uint8_t       playerMax;
  uint8_t       nbPlayer;
}               t_room;

#endif
