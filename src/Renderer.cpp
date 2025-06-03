#include "Renderer.h"

void Renderer::setWindow(sf::RenderWindow* win) {
	window = win;
}

void Renderer::clear(const sf::Color& color) {
	if (window)
		window->clear(color);
}

void Renderer::display() {
	if (window)
		window->display();
}

void Renderer::drawSprite(const sf::Sprite& sprite) {
	if (window)
		window->draw(sprite);
}

void Renderer::drawText(const sf::Text& text) {
	if (window)
		window->draw(text);
}

void Renderer::drawCircle(const sf::CircleShape& circle) {
	if (window)
		window->draw(circle);
}

void Renderer::drawRectangle(const sf::RectangleShape & rectangle) {
	if (window)
		window->draw(rectangle);
}