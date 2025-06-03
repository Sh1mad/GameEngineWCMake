#pragma once
#include <SFML/Graphics.hpp>

class EditorUI {
public:
    explicit EditorUI(sf::RenderWindow& window);
    void Update(sf::Time deltaTime);
    void Render();


    void DrawMenuBar();
    void DrawSceneHierarchy();
    void DrawInspector();

private:
    sf::RenderWindow& m_window;
};