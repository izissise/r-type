#ifndef _RESSOURCEMANAGER_HPP_
# define _RESSOURCEMANAGER_HPP_

# include <SFML/Graphics.hpp>
# include <memory>
# include "Singleton.hpp"

class RessourceManager: public Singleton<RessourceManager>
{
public:

  std::shared_ptr<sf::Font>     &getFont(const std::string &);
  std::shared_ptr<sf::Texture>  &getTexture(const std::string &);
  void                          save(const std::shared_ptr<sf::Texture> &ptr);
  void                          save(const std::shared_ptr<sf::Image> &ptr);

private:
  std::map<std::string, std::shared_ptr<sf::Font>>      _fonts;
  std::map<std::string, std::shared_ptr<sf::Texture>>   _textures;
  std::vector<std::shared_ptr<sf::Texture>>             _saveTexture;
  std::vector<std::shared_ptr<sf::Image>>             _saveImage;
};

#endif
