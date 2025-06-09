#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "EditorUI.h"

class WindowRenderer{
public:
    explicit WindowRenderer(sf::RenderWindow& window, ProjectManager& projectManager);

    // Отрисовка окна с текущим состоянием
    AppState render(AppState currentState);

private:
    sf::RenderWindow& m_window; // Ссылка на SFML-окно
    ProjectManager m_projectManager;
    EditorUI editorUI;

    AppState renderMainMenu();
    AppState renderNewProjectMenu();
    AppState renderOpenProjectMenu();
    AppState renderEditor();

    // Поле ввода для имени проекта
    char m_projectName[128] = "MyGame";

    // Поле ввода для пути к проекту
    char m_projectPath[128] = "../projects/";

};