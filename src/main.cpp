#include <iostream>
#include "ProjectManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "WindowRenderer.h"
#include "EditorUI.h"

#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game Engine");

    ImGui::SFML::Init(window);

    ProjectManager projectManager("../projects/TestProject/");
    projectManager.openProject("../projects/TestProject/project.json");
    EntityManager& entityManager = projectManager.getEntityManager();

    EditorUI editorUI(window, entityManager, projectManager);

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        window.clear(sf::Color::Black);

        editorUI.render(); // Вызов всех окон GUI

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}