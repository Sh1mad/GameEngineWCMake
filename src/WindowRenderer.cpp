#include "WindowRenderer.h"
#include <imGui.h>
#include <imgui-SFML.h>
#include <tinyfiledialogs.h>

WindowRenderer::WindowRenderer(sf::RenderWindow& window, ProjectManager& projectManager)
    : m_window(window), editorUI(window, projectManager), m_projectManager(projectManager) {}

AppState WindowRenderer::render(AppState currentState) {
    AppState nextState;

    if (currentState != AppState::Editor) {
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
    } else {
        // Если мы уже в Editor, то MainMenu не отрисовываем
        editorUI.setAppState(AppState::Editor);
        nextState = renderEditor();
    }

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

AppState WindowRenderer::renderNewProjectMenu() {
    ImGui::Text("Create New Project");
    ImGui::Separator();

    // Ввод имени проекта
    static char projectName[128] = "MyGame";
    ImGui::InputText("Project Name", projectName, IM_ARRAYSIZE(projectName));

    // Выбор пути проекта
    static char projectPath[256] = "../projects/";
    if (ImGui::Button("Browse...")) {
        const char* path = tinyfd_selectFolderDialog("Choose Folder", "../projects/");
        if (path) {
            std::strncpy(projectPath, path, IM_ARRAYSIZE(projectPath));
        }
    }

    ImGui::SameLine();
    ImGui::InputText("##ProjectPath", projectPath, IM_ARRAYSIZE(projectPath), ImGuiInputTextFlags_ReadOnly);

    // Настройки окна
    static int width = 800;
    static int height = 600;
    static bool fullscreen = false;

    ImGui::InputInt("Width", &width);
    ImGui::InputInt("Height", &height);
    ImGui::Checkbox("Fullscreen", &fullscreen);

    // Кнопки управления
    if (ImGui::Button("Create", {120, 0})) {
        try {
            // Установка пути к проекту
            m_projectManager.setProjectDir(std::string(projectPath) + "/" + projectName);

            // Создание проекта
            m_projectManager.createNewProject(projectName, width, height, fullscreen);
            return AppState::Editor;
        } catch (const std::exception& e) {
            ImGui::OpenPopup("Error");
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Back", {120, 0})) {
        return AppState::MainMenu;
    }

    // Диалог ошибок
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Failed to create project.\nCheck:\n- Path is valid\n- No existing folder with this name");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    return AppState::NewProject;
}

AppState WindowRenderer::renderOpenProjectMenu() {
    ImGui::Text("Open Existing Project");
    ImGui::Separator();

    static char projectPath[256] = "../projects/";

    if (ImGui::Button("Browse...", {120, 0})) {
        const char* filters[] = {"*.json"};
        const char* result = tinyfd_openFileDialog(
            "Select project.json",
            "", 
            1, 
            filters,
            "Project files (*.json)",
            0
        );

        if (result) {
            std::strncpy(projectPath, result, IM_ARRAYSIZE(projectPath));
        }
    }

    ImGui::SameLine();
    ImGui::InputText("##ProjectPath", projectPath, IM_ARRAYSIZE(projectPath), ImGuiInputTextFlags_ReadOnly);

    if (ImGui::Button("Open", {120, 0})) {
        try {
            std::string filePath = projectPath;
            std::string projectDir = filePath;
            if (projectDir.find_last_of("/") != std::string::npos)
                projectDir = projectDir.substr(0, projectDir.find_last_of("/"));

            m_projectManager.setProjectDir(projectDir);
            if (m_projectManager.openProject(filePath)) {
                return AppState::Editor;
            }
        } catch (...) {
            ImGui::OpenPopup("Error");
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Back", {120, 0})) {
        return AppState::MainMenu;
    }

    // Диалог ошибок
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Could not open project.\nCheck:\n- File exists\n- It's a valid project file");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    return AppState::OpenProject;
}

AppState WindowRenderer::renderEditor() {
    return editorUI.render();
}