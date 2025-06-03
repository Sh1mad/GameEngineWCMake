#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

class InputManager {
public:
	// Конструктор
	InputManager() = default;

	// Установка окна для обработки ввода
	void setWindow(sf::RenderWindow* window);

	// Обработка событий
	void pollEvents();

	// Перегруженная обработка событий
	void pollEvents(const sf::Event& event);

	// Проверка состояния клавиш
	bool isKeyPressed(sf::Keyboard::Key key) const;

	// Проверка состояния кнопок мыши
	bool isMouseButtonPressed(sf::Mouse::Button button) const;

	// Получение позиции мыши относительно окна
	sf::Vector2i getMousePosition() const;

private:
	sf::RenderWindow* window = nullptr;

	// Состояние клавиш (true - нажата, false - нет)
	std::unordered_map<sf::Keyboard::Key, bool> keyStates;

	// Состояние кнопок мыши
	std::unordered_map<sf::Mouse::Button, bool> mouseButtonStates;

};

#endif // !INPUT_MANAGER_H
