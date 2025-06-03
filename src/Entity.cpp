#include "Entity.h"

// Конструктор по умолчанию
Entity::Entity() {
    // Можно установить стандартную текстуру или оставить пустым
}

// Конструктор с текстурой и начальными координатами
Entity::Entity(sf::Texture& texture, float x, float y, float speed_x, float speed_y)
    : speed(speed_x, speed_y), position(x, y) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
    moveable = true;
}

// Установка скорости
void Entity::setSpeed(const sf::Vector2f& newSpeed) {
    speed = newSpeed;
}

// Получение скорости
const sf::Vector2f& Entity::getSpeed() const {
    return speed;
}

// Установка позиции
void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

// Получение позиции
const sf::Vector2f& Entity::getPosition() const {
    return position;
}

// Включение/отключение движения
void Entity::makeMoveable() {
    moveable = true;
}

void Entity::makeStatic() {
    moveable = false;
}

bool Entity::checkMoveable() const {
    return moveable;
}

// Остановка по осям
void Entity::stopX() {
    speed.x = 0.0f;
}

void Entity::stopY() {
    speed.y = 0.0f;
}

// Обновление состояния объекта
void Entity::update(float deltaTime) {
    // Здесь можно добавить логику анимации, если нужно
    // Например: animation.update(deltaTime);
}

// Геттеры для графики
sf::Sprite* Entity::getSprite() {
    return &sprite;
}

const sf::Texture* Entity::getTexture() const {
    return sprite.getTexture();
}

// Установка текстуры
void Entity::setTexture(const sf::Texture& texture) {
    sprite.setTexture(texture);
}