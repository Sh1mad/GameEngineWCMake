#include "WindowRenderer.h"
#include <imGui.h>
#include <imgui-SFML.h>

WindowRenderer::WindowRenderer(sf::RenderWindow& window)
    : m_window(window) {}

AppState WindowRenderer::render(AppState currentState){
    AppState nextState;

    ImGui::SetNextWindowSize({400, 300}, ImGuiCond_Once);
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize);

    switch(currentState){
        case AppState::MainMenu:
            nextState = renderMainMenu();
            break;
        case AppState::NewProject:
            nextState = renderNewProjectMenu();
            break;
        case AppState::OpenProject:
            nextState = renderOpenProjectMenu();
            break;
        default:
            nextState = AppState::MainMenu;
    }

    ImGui::End();
    
    return nextState;
}

AppState WindowRenderer::renderMainMenu(){
    ImGui::Text("Welcome to GameEngine!");
    ImGui::Separator();

    if (ImGui::Button("New project")){
        return AppState::NewProject;
    }
    if (ImGui::Button("Open project")){
        return AppState::OpenProject;
    }
    if (ImGui::Button("Exit")){
        m_window.close();
        return AppState::MainMenu;
    }

    return AppState::MainMenu;
}

AppState WindowRenderer::renderNewProjectMenu(){
    ImGui::Text("New project creation");
    ImGui::Separator();

    ImGui::InputText("Name of project", m_projectName, IM_ARRAYSIZE(m_projectName));
    ImGui::InputText("Path", m_projectPath, IM_ARRAYSIZE(m_projectPath));

    if (ImGui::Button("View...")){
        // Какой-то диалог выбора папки
    }

    if (ImGui::Button("Create")){
        // Логика создания проекта (можно передать данные в ProjectManager)
        return AppState::Editor;
    }

    ImGui::SameLine();

    if (ImGui::Button("Back")){
        return AppState::MainMenu;
    }

    return AppState::NewProject;
}

AppState WindowRenderer::renderOpenProjectMenu(){
    static char projectPah[256] = "./projects/";

    ImGui::Text("Open existing project");
    ImGui::Separator();

    ImGui::InputText("Path to project", projectPah, IM_ARRAYSIZE(projectPah));
    
    if (ImGui::Button("View...")){
        // Надо реализовать выбор файла
    }

    if (ImGui::Button("Open")){
        // Логика загрузки проекта
        return AppState::Editor;
    }

    ImGui::SameLine();

    if (ImGui::Button("Back")){
        return AppState::MainMenu;
    }

    return AppState::OpenProject;
}