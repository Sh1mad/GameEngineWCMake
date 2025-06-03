#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

class Renderer {
public:
	// Конструктор
	Renderer() = default;

	// Установка окна для рендера
	void setWindow(sf::RenderWindow* window);

	// Очистка экрана
	void clear(const sf::Color& color = sf::Color::Black);

	// Отображение кадра
	void display();

	// Отрисовка спрайта
	void drawSprite(const sf::Sprite& sprite);

	// Отрисовка текста
	void drawText(const sf::Text& text);

	// Отрисовка прямоуголника
	void drawRectangle(const sf::RectangleShape& rectangle);

	// Отрисовка круга
	void drawCircle(const sf::CircleShape& circle);

private:
	sf::RenderWindow* window = nullptr; // Указатель на окно
};

#endif // !RENDERER_H
