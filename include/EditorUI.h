#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "ProjectManager.h"
#include <imgui.h>
#include <imgui-SFML.h>

enum class AppState{
    MainMenu,
    NewProject,
    OpenProject,
    Editor
};

class EditorUI {
public:
    explicit EditorUI(sf::RenderWindow& window, ProjectManager& projectManger);

    void update(sf::Time dt);
    AppState render();

    void setAppState(AppState appState); // Для связи с WindowRenderer

private:
    sf::RenderWindow& m_window;
    EntityManager& m_entityManager;
    ProjectManager& m_projectManager;

    std::shared_ptr<Entity> selectedEntity = nullptr;
    std::string selectedTextureName;
    std::string newEntityName = "NewEntity";
    std::string selectedTextureNameForEntity;
    
    // Флаги для управления открытыми окнами
    bool showTextureManager = false;
    bool showSceneHierarchy = false;
    bool showCreateEntityWindow = false;
    bool showScenePreviewWindow = true;
    bool isDraggingEntity = false;              // Флаг для перетаскивания сущности

    ImVec2 dragStartPos;           // Начальная позиция клика

    void drawMenuBar();                    // Функция для отрисовки панели меню в редакторе
    void drawSceneHierarchy();            // Функция для отрисовки окна со списком сущностей
    void drawInspectorWindow();            // Функция для просмотра и редактирования свойств конкретной сущности
    void drawScenePreview();
    void drawToolBar();                    // Функция для вывода панели инструментов
    void drawTextureManager();             // Функция для вывода окна с текстурами
    void drawCreateEntityWindow();         // Функция для вывода окна создания сущности

    AppState m_appState = AppState::Editor;        // Указатель для определения текущего состояния системы
};