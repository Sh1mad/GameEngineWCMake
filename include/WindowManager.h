#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>


class WindowManager {
public:
    // Конструктор
    WindowManager() = default;

    // Создание окна
    void createWindow(const sf::String & title, unsigned width, unsigned height, bool fullscreen = false);

    // Закрытие окна
    void closeWindow();

    // Получение ссылки на окно
    sf::RenderWindow& getWindow();

    // Получение указателя на окно
    sf::RenderWindow* getWindowPtr();

    // Проверка, открыто ли окно
    bool isWindowOpen();

private:
	sf::RenderWindow window; // Объект окна
	bool isFullScreen = false;
};

#endif // WINDOW_MANAGER_H