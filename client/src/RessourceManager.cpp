#include "RessourceManager.hpp"

std::shared_ptr<sf::Font>     &RessourceManager::getFont(const std::string &key)
{
  try
  {
    return (_fonts.at(key));
  }
  catch (std::exception &e)
  {
    auto tmp = std::make_shared<sf::Font>();

    if (!tmp->loadFromFile(key))
      throw std::runtime_error("Cannot load the font");
    _fonts.insert(std::pair<std::string, std::shared_ptr<sf::Font>>(key, tmp));
    return (_fonts[key]);
  }
}

std::shared_ptr<sf::Texture>  &RessourceManager::getTexture(const std::string &key)
{
  try
  {
    return (_textures.at(key));
  }
  catch (std::exception &e)
  {
    auto tmp = std::make_shared<sf::Texture>();

    if (!tmp->loadFromFile(key))
      throw std::runtime_error("Cannot load the font");
    _textures.insert(std::pair<std::string, std::shared_ptr<sf::Texture>>(key, tmp));
    return (_textures[key]);
  }
}


