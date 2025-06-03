#include "InputManager.h"

void InputManager::setWindow(sf::RenderWindow* win) {
	window = win;
}

void InputManager::pollEvents() {
    if (!window) return;

    sf::Event event;
    while (window->pollEvent(event)) {
        pollEvents(event); // Вызов перегруженного метода
    }
}

void InputManager::pollEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        keyStates[event.key.code] = true;
    }
    if (event.type == sf::Event::KeyReleased) {
        keyStates[event.key.code] = false;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        mouseButtonStates[event.mouseButton.button] = true;
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        mouseButtonStates[event.mouseButton.button] = false;
    }

}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const{
    auto it = keyStates.find(key);
    return (it != keyStates.end()) && it->second;
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button) const {
    auto it = mouseButtonStates.find(button);
    return (it != mouseButtonStates.end()) && it->second;
}

sf::Vector2i InputManager::getMousePosition() const {
    if (window) {
        return sf::Mouse::getPosition(*window);
    }
    return sf::Vector2i(0, 0);
}
