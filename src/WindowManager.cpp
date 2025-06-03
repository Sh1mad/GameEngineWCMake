#include "WindowManager.h"
#include <stdexcept>

void WindowManager::createWindow(const sf::String& title, unsigned width, unsigned height, bool fullscreen) {
    // Проверка параметров
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Размер окна должен быть больше нуля!");
    }

    // Создание окна
    if (fullscreen) {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode(desktop.width, desktop.height), title, sf::Style::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Resize);
    }

    // Проверка успешности создания окна
    if (!window.isOpen()) {
        throw std::runtime_error("Не удалось создать окно!");
    }

    // Настройка вертикальной синхронизации
    //window.setVerticalSyncEnabled(true);

    isFullScreen = fullscreen;
}

void WindowManager::closeWindow() {
	if (window.isOpen()) {
		window.close();
	}
}

sf::RenderWindow& WindowManager::getWindow() {
	return window;
}

sf::RenderWindow* WindowManager::getWindowPtr() {
    return &window;
}

bool WindowManager::isWindowOpen() {
	return window.isOpen();
}