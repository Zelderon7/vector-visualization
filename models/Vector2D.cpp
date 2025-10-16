//
// Created by zelderon on 10/7/25.
//

#include "Vector2D.h"

#include <vector>
#include <SFML/Graphics.hpp>


Vector2D::Vector2D(float x, float y, sf::Color color, const Vector2D* previous)
    :x(x), y(y), color(color), previous(previous)
{
}

Vector2D::Vector2D(float x, float y, sf::Color color)
    :Vector2D(x, y, color, nullptr) {}

Vector2D::Vector2D(float x, float y)
    :Vector2D(x, y, sf::Color::White) { }

Vector2D::Vector2D(sf::Vector2<float> value, sf::Color color, const Vector2D *previous)
    :Vector2D(value.x, value.y, color, previous) { }

void Vector2D::setX(float x) {
    this->x = x;
}

void Vector2D::setY(float y) {
    this->y = y;
}

void Vector2D::setColor(const sf::Color color) {
    this->color = color;
}

void Vector2D::setValue(sf::Vector2<float> pos) {
    this->x = pos.x;
    this->y = pos.y;
}

void Vector2D::setPrevious(const Vector2D *previous) {
    this->previous = previous;
}

float Vector2D::getX() const {
    return x;
}

float Vector2D::getY() const {
    return y;
}

sf::Color Vector2D::getColor() const {
    return color;
}

sf::Vector2<float> Vector2D::getStartPos() const {
    return previous ?
        previous->getEnd() :
        sf::Vector2<float>(0, 0);
}

sf::Vector2<float> Vector2D::getValue() const {
    return {x, y};
}

sf::Vector2<float> Vector2D::getEnd() const {
    return getStartPos() + (sf::Vector2<float>){x, y};
}

Vector2D Vector2D::operator*(const float scalar) const {
    return Vector2D(x * scalar, y * scalar, color, previous);
}

Vector2D Vector2D::operator+(const Vector2D &other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D &other) const {
    return *this + (other * -1);
}

Vector2D::operator sf::Vector2<float>() const {
    return {x, y};
}
