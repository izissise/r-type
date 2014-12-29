#ifndef _INPUT_HPP_
# define _INPUT_HPP_

#include <SFML/Graphics.hpp>
#include <chrono>
#include "Vector.hpp"

class Input
{
public:
    Input();
    ~Input();

    bool getState(sf::Keyboard::Key key) const;
    bool getState(const std::string &action) const;

    void setAction(const std::string &action, sf::Keyboard::Key key);

    void update(const sf::Event &event);

    bool isButtonPressed() const { return _isButtonPressed; };
    bool isMouseWheeled() const { return _isMouseWheeled; };

    Vector<int> getMousePos() const { return {_x, _y}; };
    Vector<int> getMouseRelPos() const { return {_xRel, _yRel}; };
    sf::Mouse::Button getButtonPressed() const { return _button; };
    int getWheel() const { return _delta; };
    const std::string &getText() const { return _text; };

protected:
    std::map<sf::Keyboard::Key, bool>   _keyState;
    std::map<std::string, sf::Keyboard::Key> _action;

    int _x;
    int _y;
    int _xRel;
    int _yRel;

    bool _isButtonPressed;
    sf::Mouse::Button _button;

    bool _isMouseWheeled;
    int _delta;
    std::chrono::steady_clock::time_point _chronoWheel;

    std::string _text;
    std::chrono::steady_clock::time_point _chronoText;
};

#endif