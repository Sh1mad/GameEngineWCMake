#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "ProjectManager.h"

class EditorUI {
public:
    explicit EditorUI(sf::RenderWindow& window, EntityManager& entityManager, ProjectManager& projectManger);

    void update(sf::Time dt);
    void render();

private:
    sf::RenderWindow& m_window;
    EntityManager& m_entityManager;
    ProjectManager& m_projectManager;

    std::shared_ptr<Entity> selectedEntity = nullptr;
    std::string selectedTextureName;

    void drawMenuBar();                    // Функция для отрисовки панели меню в редакторе
    void drawHierarchyWindow();            // Функция для отрисовки окна со списком сущностей
    void drawInspectorWindow();            // Функция для просмотра и редактирования свойств конкретной сущности
    void drawGameView();
    void drawToolBar();                    // Функция для вывода панели инструментов
    void drawTextureManager();             // Функция для вывода окна с текстурами
};