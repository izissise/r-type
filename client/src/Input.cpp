#include <iostream>
#include "Input.hpp"

Input::Input()
{
    _x = _y = _xRel = _yRel = 0;
    _isButtonPressed = false;
    _isMouseWheeled = false;
    _delta = 0;
}

Input::~Input()
{

}

bool Input::getState(sf::Keyboard::Key key) const
{
    try
    {
        return (_keyState.at(key));
    }
    catch (std::exception &e)
    {
        return (false);
    }
}

bool Input::getState(const std::string &action) const
{
    sf::Keyboard::Key key;
    try
    {
        key = _action.at(action);
    }
    catch (std::exception &e)
    {
        return (false);
    }
    try
    {
        return (_keyState.at(key));
    }
    catch (std::exception &e)
    {
        return (false);
    }
}

void Input::setAction(const std::string &action, sf::Keyboard::Key key)
{
    _action[action] = key;
}

void Input::update(const sf::Event &event)
{
    if (_isMouseWheeled && (std::chrono::steady_clock::now() - _chronoWheel).count() >= 0.3)
        _isMouseWheeled = false;
    _text = "";
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            _keyState[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            _keyState[event.key.code] = false;
            break;
        case sf::Event::TextEntered:
            _text += event.text.unicode;
            _chronoText = std::chrono::steady_clock::now();
            break;
        case sf::Event::MouseButtonPressed:
            _isButtonPressed = true;
            _xRel = _x - event.mouseButton.x;
            _yRel = _y - event.mouseButton.y;
            _x = event.mouseButton.x;
            _y = event.mouseButton.y;
            _button = event.mouseButton.button;
            break;
        case sf::Event::MouseButtonReleased:
            _isButtonPressed = false;
            break;
        case sf::Event::MouseMoved:
            _xRel = _x - event.mouseMove.x;
            _yRel = _y - event.mouseMove.y;
            _x = event.mouseMove.x;
            _y = event.mouseMove.y;
            break;
        case sf::Event::MouseWheelMoved:
            _isMouseWheeled = true;
            _delta = event.mouseWheel.delta;
            _chronoWheel = std::chrono::steady_clock::now();
        default:
            break;
    }
}
