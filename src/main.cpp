#include <iostream>
#include "ProjectManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "WindowRenderer.h"
#include "EditorUI.h"

#include <imgui.h>
#include <imgui-SFML.h>

// int main() {
//     sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game Engine");

//     ImGui::SFML::Init(window);

//     ProjectManager projectManager("../projects/TestProject/");
//     projectManager.openProject("../projects/TestProject/project.json");
//     EntityManager& entityManager = projectManager.getEntityManager();

//     WindowRenderer windowRenderer(window, entityManager, projectManager);
//     AppState state = AppState::MainMenu;
//     sf::Clock deltaClock;

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             ImGui::SFML::ProcessEvent(event);
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         sf::Time dt = deltaClock.restart();
//         ImGui::SFML::Update(window, dt);

//         window.clear(sf::Color::Black);

//         state = windowRenderer.render(state);

//         window.display();
//     }

//     ImGui::SFML::Shutdown();
//     return 0;
// }

// int main() {
//     sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game Engine");
//     ImGui::SFML::Init(window);

//     ProjectManager projectManager("../projects/TestProject/");

//     WindowRenderer windowRenderer(window, projectManager.getEntityManager(), projectManager);
//     AppState state = AppState::MainMenu;

//     sf::Clock deltaClock;
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             ImGui::SFML::ProcessEvent(event);
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         ImGui::SFML::Update(window, deltaClock.restart());

//         state = windowRenderer.render(state); // Рендер текущего окна

//         window.clear();
//         ImGui::Render();
//         ImGui::SFML::Render(window);
//         window.display();
//     }

//     ImGui::SFML::Shutdown();
//     return 0;
// }

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Engine IDE");
    ImGui::SFML::Init(window);

    ProjectManager projectManager; // ← используем дефолтный конструктор
    WindowRenderer renderer(window, projectManager);
    AppState state = AppState::MainMenu;

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        state = renderer.render(state); // рендер меню / редактора

        window.clear();
        ImGui::Render();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}