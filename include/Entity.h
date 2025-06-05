#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    // Конструктор по умолчанию
    Entity();

    // Конструктор с параметрами без указания области на текстуре (полная текстура)
    Entity(const std::string& texturePath, int ID, float x = 0, float y = 0, float speed_x = 0, float speed_y = 0);

    // Конструктор с параметрами с указание области на текстуре
    Entity(const std::string& texturePath, const sf::IntRect& textureRect, int ID, float x = 0, float y = 0, float speed_x = 0, float speed_y = 0);

    // Установка скорости
    void setSpeed(const sf::Vector2f& speed);

    // Получение скорости
    const sf::Vector2f& getSpeed() const;

    // Установка позиции
    void setPosition(const sf::Vector2f& pos);

    // Получение позиции
    const sf::Vector2f& getPosition() const;

    // Включение/отключение возможности движения
    void makeMoveable();
    void makeStatic();
    bool checkMoveable() const;

    // Остановка движения по осям
    void stopX();
    void stopY();

    // Обновление состояния объекта (можно использовать для анимаций)
    void update(float deltaTime);

    void setTextureRect(const sf::IntRect& rect); // Задать регион на текстуре
    const sf::IntRect& getTextureRect() const; // Геттер региона

    // Геттер для пути к текстуре
    std::string getTexturePath() const;

    // Геттеры для графики
    sf::Sprite* getSprite();
    const sf::Texture* getTexture() const;

    // Установка текстуры
    void setTexture(const sf::Texture& texture);

    // Установка ID
    void setId(int ID);

    // Геттер ID
    int getId() const;
private:
    std::string texturePath;
    int id;                       // ID сущности
    sf::Sprite sprite;            // Спрайт объекта
    sf::Vector2f speed;           // Скорость
    sf::Vector2f position;        // Позиция
    bool moveable = false;        // Возможность двигаться
};

#endif // ENTITY_H