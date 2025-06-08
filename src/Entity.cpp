#include "Entity.h"
#include "ResourceManager.h"

// Конструктор по умолчанию
Entity::Entity() {
    // Можно установить стандартную текстуру или оставить пустым
}

// Конструктор с текстурой по имени
Entity::Entity(const std::string& textureName, int ID, float x, float y, float speed_x, float speed_y)
    : speed(speed_x, speed_y), position(x, y), id(ID), textureName(textureName)
{
    const sf::Texture& texture = ResourceManager::getTexture(textureName);
    sprite.setTexture(texture);
    sprite.setPosition(position);
    moveable = true;
}

// Конструктор с регионом текстуры
Entity::Entity(const std::string& textureName, const sf::IntRect& textureRect, int ID,
               float x, float y, float speedX, float speedY)
    : textureName(textureName), speed(speedX, speedY), position(x, y), id(ID)
{
    const sf::Texture& texture = ResourceManager::getTexture(textureName);
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setPosition(x, y);
    moveable = true;
}

// Геттер пути к текстуре
std::string Entity::getTexturePath() const{
    return ResourceManager::getPath(textureName);
}

// Задание региона текстуры
void Entity::setTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
}

// Получение региона текстуры
const sf::IntRect& Entity::getTextureRect() const {
    return sprite.getTextureRect();
}

// Сеттер ID
void Entity::setId(int ID){
    id = ID;
}

// Геттер ID
int Entity::getId() const{
    return id;
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

void Entity::setTextureName(const std::string& name) {
    if (name == textureName)
        return;

    const sf::Texture& newTexture = ResourceManager::getTexture(name);
    sprite.setTexture(newTexture);
    textureName = name;
}

const std::string& Entity::getTextureName() const {
    return textureName;
}

void Entity::setTexture(const std::string& textureName) {
    this->setTextureName(textureName);
}