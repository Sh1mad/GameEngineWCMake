#pragma once
#define TINYFD_WIN_CONSOLE

#include "EditorUI.h"
#include <iostream>
#include "ResourceManager.h"
#include <tinyfiledialogs.h> // Библиотека для работы с файлами в файловой системе

EditorUI::EditorUI(sf::RenderWindow& window, EntityManager& entityManager, ProjectManager& projectManager)
    : m_window(window), m_entityManager(entityManager), m_projectManager(projectManager){}

void EditorUI::update(sf::Time dt){
    ImGui::SFML::Update(m_window, dt);
}

void EditorUI::render() {
    drawMenuBar();
    drawToolBar();

    if (showTextureManager) drawTextureManager();
    if (showSceneHierarchy) drawSceneHierarchy();
    if (selectedEntity) drawInspectorWindow();
    if (showCreateEntityWindow) drawCreateEntityWindow();
    if (showScenePreviewWindow) drawScenePreview();

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

    if (ImGui::Button("Textures")) {
        showTextureManager = !showTextureManager;
    }
    ImGui::SameLine();

    if (ImGui::Button("Entities")) {
        showSceneHierarchy = !showSceneHierarchy;
    }
    ImGui::SameLine();

    if (ImGui::Button("Add Entity")) {
        showCreateEntityWindow = !showCreateEntityWindow;
    }
    ImGui::SameLine();

    if (ImGui::Button("Scene Preview")) {
        showScenePreviewWindow = !showScenePreviewWindow;
    }
    ImGui::SameLine();

    ImGui::End();

    // === Диалог "No entity selected" ===
    if (ImGui::BeginPopupModal("No entity selected", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Choose entity from the list.");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EditorUI::drawSceneHierarchy() {
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
    selectedEntity->setTextureRect(sf::IntRect(r_left, r_top, r_width, r_height));

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

void EditorUI::drawCreateEntityWindow() {
    ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Create New Entity", &showCreateEntityWindow)) {

        static char newEntityName[128] = "";   // Имя сущности
        std::string newEntityNameStr;

        // Поле ввода имени сущности
        ImGui::InputText("Entity Name", newEntityName, IM_ARRAYSIZE(newEntityName));

        // Выбор текстуры из списка
        const auto& allTextures = ResourceManager::getAllTextures();
        std::vector<std::string> textureNames;
        for (const auto& [name, info] : allTextures) {
            textureNames.push_back(name);
        }

        static int selectedTextureIndex = 0;
        if (!textureNames.empty()) {
            const char* previewValue = textureNames[selectedTextureIndex].c_str();
            if (ImGui::BeginCombo("Select Texture", previewValue)) {
                for (int i = 0; i < textureNames.size(); i++) {
                    bool isSelected = (i == selectedTextureIndex);
                    if (ImGui::Selectable(textureNames[i].c_str(), isSelected)) {
                        selectedTextureIndex = i;
                        selectedTextureNameForEntity = textureNames[i];
                    }
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        } else {
            ImGui::Text("No textures loaded");
        }

        // Поля ввода координат
        static float x = 0.0f, y = 0.0f;
        ImGui::InputFloat("X Position", &x);
        ImGui::InputFloat("Y Position", &y);

        // Поля ввода скорости (опционально)
        static float vx = 0.0f, vy = 0.0f;
        ImGui::InputFloat("Speed X", &vx);
        ImGui::InputFloat("Speed Y", &vy);

        // Кнопка создания
        if (ImGui::Button("Create", ImVec2(-1, 0))) {
            try {
                int newID = m_entityManager.getNextId(); // или любая логика генерации ID
                auto entity = std::make_shared<Entity>(
                    selectedTextureNameForEntity,
                    newID,
                    x, y,
                    vx, vy
                );
                entity->makeMoveable();
                m_entityManager.addEntity(entity, newEntityNameStr.assign(newEntityName));
                showCreateEntityWindow = false;
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при создании сущности: " << e.what() << std::endl;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(-1, 0))) {
            showCreateEntityWindow = false;
        }

    }
    ImGui::End();
}

void EditorUI::drawScenePreview() {
    ImGui::Begin("Scene Preview");

    static float sceneScale = 0.5f;
    static float previewWidth = 600.0f;
    static float previewHeight = 400.0f;

    ImGui::SliderFloat("Scale", &sceneScale, 0.25f, 2.0f);
    ImGui::SliderFloat("Width", &previewWidth, 200.0f, 1000.0f);
    ImGui::SliderFloat("Height", &previewHeight, 200.0f, 800.0f);

    ImVec2 sceneSize(previewWidth, previewHeight);
    ImGui::InvisibleButton("##scene", sceneSize);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 scenePos = ImGui::GetItemRectMin();

    // === Сетка фона (необязательно) ===
    for (int x = 0; x < previewWidth; x += 32) {
        drawList->AddLine({scenePos.x + x, scenePos.y}, {scenePos.x + x, scenePos.y + previewHeight}, IM_COL32(100, 100, 100, 50));
    }
    for (int y = 0; y < previewHeight; y += 32) {
        drawList->AddLine({scenePos.x, scenePos.y + y}, {scenePos.x + previewWidth, scenePos.y + y}, IM_COL32(100, 100, 100, 50));
    }

    // === Отрисовка сущностей ===
    for (const auto& entity : m_entityManager.getEntities()) {
        const sf::Sprite& sprite = *entity->getSprite();
        const sf::Texture* texture = sprite.getTexture();
        if (!texture) continue;

        const sf::IntRect& rect = sprite.getTextureRect();
        sf::Vector2f pos = entity->getPosition();

        // UV координаты текстуры
        ImVec2 uv0(
            static_cast<float>(rect.left) / texture->getSize().x,
            static_cast<float>(rect.top) / texture->getSize().y
        );
        ImVec2 uv1(
            static_cast<float>(rect.left + rect.width) / texture->getSize().x,
            static_cast<float>(rect.top + rect.height) / texture->getSize().y
        );

        // Позиция и размер с масштабированием
        ImVec2 scaledPosition(scenePos.x + pos.x * sceneScale, scenePos.y + pos.y * sceneScale);
        ImVec2 size(rect.width * sceneScale, rect.height * sceneScale);

        // Текстура
        ImTextureID textureID = (ImTextureID)(texture->getNativeHandle());

        // Отрисовка текстуры
        drawList->AddImage(
            textureID,
            scaledPosition,
            {scaledPosition.x + size.x, scaledPosition.y + size.y},
            uv0,
            uv1,
            IM_COL32_WHITE
        );

        // Обводка выбранного объекта
        if (selectedEntity == entity) {
            drawList->AddRect(scaledPosition, {scaledPosition.x + size.x, scaledPosition.y + size.y}, IM_COL32(255, 255, 0, 128), 0, 0, 2.0f);
        }

        // === Проверяем, находится ли курсор над сущностью ===
        bool isOverEntity = false;
        ImVec2 currentMousePos = ImGui::GetMousePos();

        if (currentMousePos.x >= scaledPosition.x && 
            currentMousePos.x <= scaledPosition.x + size.x &&
            currentMousePos.y >= scaledPosition.y && 
            currentMousePos.y <= scaledPosition.y + size.y)
        {
            isOverEntity = true;
        }

        // === Обработка клика для выбора сущности ===
        if (ImGui::IsMouseClicked(0) && isOverEntity) {
            selectedEntity = entity;
        }

        // === Обработка перетаскивания ===
        if (ImGui::IsMouseDragging(0) && selectedEntity == entity && isOverEntity) {
            ImVec2 delta = ImGui::GetMouseDragDelta(0);
            sf::Vector2f newPosition(
                pos.x + delta.x / sceneScale,
                pos.y + delta.y / sceneScale
            );
            entity->setPosition(newPosition);
            ImGui::ResetMouseDragDelta(); // Чтобы не скакала позиция
        }
    }

    ImGui::End();
    
}