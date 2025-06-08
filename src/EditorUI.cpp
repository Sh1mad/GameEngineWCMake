#pragma once
#define TINYFD_WIN_CONSOLE

#include "EditorUI.h"
#include <iostream>
#include "ResourceManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <tinyfiledialogs.h> // Библиотека для работы с файлами в файловой системе

EditorUI::EditorUI(sf::RenderWindow& window, EntityManager& entityManager, ProjectManager& projectManager)
    : m_window(window), m_entityManager(entityManager), m_projectManager(projectManager){}

void EditorUI::update(sf::Time dt){
    ImGui::SFML::Update(m_window, dt);
}

void EditorUI::render() {
    drawMenuBar();
    drawToolBar();
    drawHierarchyWindow();
    drawInspectorWindow();
    drawTextureManager();
    //drawGameView();

    ImGui::Render();
    ImGui::SFML::Render(m_window);
}

// ======== PRIVATE METHODS ===========

void EditorUI::drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New project")) {
                // TODO: переключить состояние приложения
            }
            if (ImGui::MenuItem("Open project")) {
                // TODO: вызвать openProject()
            }
            if (ImGui::MenuItem("Save scene")) {
                m_projectManager.saveCurrentProject();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Designer")) {
            if (ImGui::MenuItem("Project settings")) {}
            if (ImGui::MenuItem("Plaggins")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void EditorUI::drawToolBar() {
    ImGui::SetNextWindowPos({0, 20});
    ImGui::SetNextWindowSize({m_window.getSize().x, 50});
    ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    if (ImGui::Button("Play")) {
        // TODO: Переключить на игровой режим
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {}
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {}
    ImGui::SameLine();
    if (ImGui::Button("Add Entity")) {
        // TODO: добавить новую сущность
    }

    ImGui::End();
}

void EditorUI::drawHierarchyWindow() {
    ImGui::Begin("Scene Hierarchy");

    for (auto& entity : m_entityManager.getEntities()) {
        std::string label = "Entity " + std::to_string(entity->getId());

        if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) {
            selectedEntity = entity;
        }
    }

    ImGui::End();
}

void EditorUI::drawTextureManager() {
    ImGui::Begin("Textures of project");

    static char newTextureName[128] = "";   // Имя текстуры
    static std::string newTexturePath;     // Путь к файлу

    ImGui::InputText("Texture Name", newTextureName, IM_ARRAYSIZE(newTextureName));
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        const char* fileTypes[] = {"*.png", "*.jpg", "*.gif"};
        const char* path = tinyfd_openFileDialog(
            "Select texture",
            "", 
            3, 
            fileTypes,
            "Image files (*.png *.jpg *.gif)", 
            0
        );
        if (path) {
            newTexturePath = path;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        if (newTextureName[0] != '\0' && !newTexturePath.empty()) {
            try {
                bool success = ResourceManager::loadTexture(newTextureName, newTexturePath);
                if (!success) {
                    std::cerr << "Ошибка: текстура уже существует" << std::endl;
                } else {
                    selectedTextureName = newTextureName;
                    newTexturePath.clear();
                    strcpy(newTextureName, "");
                }
            } catch (const std::exception& e) {
                std::cerr << "Не удалось загрузить текстуру: " << e.what() << std::endl;
            }
        } else {
            ImGui::Text("Enter name of texture.");
        }
    }

    ImGui::Separator();

    // === Список текстур по имени ===
    const auto& allTextures = ResourceManager::getAllTextures();
    for (const auto& [name, info] : allTextures) {
        bool isSelected = (selectedTextureName == name);
        if (ImGui::Selectable(name.c_str(), isSelected)) {
            selectedTextureName = name;
        }
    }

    ImGui::Separator();

    // === Информация о выбранной текстуре ===
    if (!selectedTextureName.empty()) {
        auto it = allTextures.find(selectedTextureName);
        if (it != allTextures.end()) {
            const sf::Texture& texture = it->second.texture;
            const std::string& realPath = it->second.filePath;

            ImTextureID textureID = (ImTextureID)(texture.getNativeHandle());

            sf::Vector2u size = texture.getSize();
            float previewWidth = 100.f;
            float scale = previewWidth / static_cast<float>(size.x);
            ImVec2 previewSize(previewWidth, static_cast<float>(size.y) * scale);

            ImGui::Text("Preview:");
            ImGui::Image(textureID, previewSize);

            ImGui::Spacing();
            ImGui::Text("Real Path: %s", realPath.c_str());
            ImGui::Text("Size: %dx%d", size.x, size.y);

            if (ImGui::Button("Delete texture")) {
                ResourceManager::removeTexture(selectedTextureName);
                selectedTextureName.clear(); // Сброс имени
            }
        }
    } else {
        ImGui::Text("Choose a texture from the list");
    }

    ImGui::End();
}

void EditorUI::drawInspectorWindow() {
    ImGui::Begin("Inspector");

    if (!selectedEntity) {
        ImGui::Text("Choose object from hierarchy");
        ImGui::End();
        return;
    }

    ImGui::Text("ID: %d", selectedEntity->getId());
    ImGui::Separator();

    // === Позиция ===
    sf::Vector2f pos = selectedEntity->getPosition();
    ImGui::InputFloat2("Position", &pos.x);
    selectedEntity->setPosition({pos.x, pos.y});

    // === Скорость ===
    sf::Vector2f speed = selectedEntity->getSpeed();
    ImGui::InputFloat2("Speed", &speed.x);
    selectedEntity->setSpeed({speed.x, speed.y});

    // === Текстура ===
    const std::string& textureName = selectedEntity->getTextureName();
    if (textureName.empty()) {
        ImGui::Text("No texture assigned");
        ImGui::End();
        return;
    }

    ImGui::Text("Texture: %s", textureName.c_str());

    // Получаем текстуру по имени
    const sf::Texture* texture = selectedEntity->getSprite()->getTexture();
    if (!texture) {
        ImGui::Text("No valid texture loaded");
        ImGui::End();
        return;
    }

    // Превью всей текстуры
    ImTextureID textureID = (ImTextureID)(texture->getNativeHandle());
    sf::Vector2u texSize = texture->getSize();
    float previewWidth = 100.f;
    float scale = previewWidth / static_cast<float>(texSize.x);
    ImVec2 previewSize(previewWidth, static_cast<float>(texSize.y) * scale);

    ImGui::Text("Texture preview:");
    ImGui::Image(textureID, previewSize);

    // === Отображение региона текстуры на превью ===
    sf::IntRect rect = selectedEntity->getTextureRect();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 imagePos = ImGui::GetItemRectMin();

    ImVec2 p_min(imagePos.x + rect.left * scale, imagePos.y + rect.top * scale);
    ImVec2 p_max(p_min.x + rect.width * scale, p_min.y + rect.height * scale);

    drawList->AddRectFilled(p_min, p_max, IM_COL32(255, 255, 0, 128)); // Жёлтый прямоугольник

    // === Редактирование региона текстуры ===
    int r_left   = rect.left;
    int r_top    = rect.top;
    int r_width  = rect.width;
    int r_height = rect.height;

    ImGui::Spacing();
    ImGui::Text("Edit texture region:");
    ImGui::InputInt("Left", &r_left);
    ImGui::InputInt("Top", &r_top);
    ImGui::InputInt("Width", &r_width);
    ImGui::InputInt("Height", &r_height);

    // Ограничение значений
    r_left = std::clamp(r_left, 0, static_cast<int>(texSize.x));
    r_top = std::clamp(r_top, 0, static_cast<int>(texSize.y));
    r_width = std::clamp(r_width, 1, static_cast<int>(texSize.x) - r_left);
    r_height = std::clamp(r_height, 1, static_cast<int>(texSize.y) - r_top);

    // Сохраняем изменения при деактивации поля ввода
    if (ImGui::IsItemDeactivated()) {
        selectedEntity->setTextureRect(sf::IntRect(r_left, r_top, r_width, r_height));
    }

    // === Состояние объекта ===
    bool moveable = selectedEntity->checkMoveable();
    if (ImGui::Checkbox("Moveable", &moveable)) {
        if (moveable)
            selectedEntity->makeMoveable();
        else
            selectedEntity->makeStatic();
    }

    ImGui::End();
}

void EditorUI::drawGameView() {
    ImGui::Begin("Level designer");

    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    ImGui::InvisibleButton("Canvas", canvasSize);
    ImVec2 canvas_p0 = ImGui::GetItemRectMin();
    ImVec2 canvas_p1 = ImGui::GetItemRectMax();

    // ImGui::SetCursorScreenPos(canvas_p0);
    // ImGui::Image((void*)(intptr_t)m_window.getSystemHandle(), canvasSize);

    // Здесь можно реализовать логику перемещения объекта мышью по сцене

    ImGui::End();
}