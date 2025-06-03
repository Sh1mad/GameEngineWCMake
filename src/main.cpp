#include "Core.h"
#include "WindowRenderer.h"
#include <imGui.h>
#include <imGui-SFML.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Engine");
    ImGui::SFML::Init(window);

    WindowRenderer windowRenderer(window);
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

        state = windowRenderer.render(state); // Рендер текущего окна

        window.clear();
        ImGui::Render();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

// int main() {
//     sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
//     window.setFramerateLimit(60);
//     ImGui::SFML::Init(window);


//     sf::Clock deltaClock;
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             ImGui::SFML::ProcessEvent(window, event);

//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//         }

//         ImGui::SFML::Update(window, deltaClock.restart());

//         //ImGui::ShowDemoWindow();

//         ImGui::Begin("Hello, world!");
//         ImGui::Button("Look at this pretty button");
//         ImGui::End();

//         window.clear();
//         ImGui::SFML::Render(window);
//         window.display();
//     }

//     ImGui::SFML::Shutdown();
// }