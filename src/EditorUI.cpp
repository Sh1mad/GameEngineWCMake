#include "EditorUI.h"
#include <imGui.h>
#include <imgui-SFML.h>

EditorUI::EditorUI(sf::RenderWindow& window) : m_window(window){
    ImGui::SFML::Init(window);
}

void EditorUI::Update(sf::Time deltaTime){
    ImGui::SFML::Update(m_window, deltaTime);
}

void EditorUI::Render(){
    ImGui::SFML::Render(m_window);
}

void EditorUI::DrawMenuBar(){
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {}
            if (ImGui::MenuItem("Load Scene")) {}
            if (ImGui::MenuItem("Save Scene")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")){}
            ImGui::EndMenu();
        }
    }
}

void EditorUI::DrawSceneHierarchy(){
    ImGui::Begin("Scene Hierarchy");
    ImGui::Text("Entities:");
    ImGui::BulletText("Player");
    ImGui::BulletText("Enemy");
    ImGui::End();
}

void EditorUI::DrawInspector() {
    ImGui::Begin("Inspector");
    ImGui::Text("Selected Entity Properties:");
    ImGui::InputFloat2("Position", nullptr);
    ImGui::InputFloat2("Speed", nullptr);
    ImGui::Checkbox("Moveable", nullptr);
    ImGui::End();
}