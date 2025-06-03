#pragma once

#include <SFML/Graphics.hpp>
#include <string>

enum class AppState{
    MainMenu,
    NewProject,
    OpenProject,
    Editor
};

class WindowRenderer{
public:
    explicit WindowRenderer(sf::RenderWindow& window);

    // Отрисовка окна с текущим состоянием
    AppState render(AppState currentState);

private:
    sf::RenderWindow& m_window; // Ссылка на SFML-окно

    AppState renderMainMenu();
    AppState renderNewProjectMenu();
    AppState renderOpenProjectMenu();

    // Поле ввода для имени проекта
    char m_projectName[128] = "MyGame";

    // Поле ввода для пути к проекту
    char m_projectPath[128] = "./projects/";

};