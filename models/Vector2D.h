//
// Created by zelderon on 10/7/25.
//

#ifndef VECTOR_CALCULATOR_VECTOR2D_H
#define VECTOR_CALCULATOR_VECTOR2D_H
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>


class Vector2D {
private:
    float x, y;
    sf::Color color;
    const Vector2D* previous;

public:
    //Constructors
    Vector2D(sf::Vector2<float> value, sf::Color color, const Vector2D* previous);
    Vector2D(float x, float y, sf::Color color, const Vector2D* previous);
    Vector2D(float x, float y, sf::Color color);
    Vector2D(float x, float y);

    //Getters and Setters
    void setX(float x);
    void setY(float y);
    void setValue(sf::Vector2<float> pos);
    void setColor(sf::Color color);
    void setPrevious(const Vector2D* previous);

    float getX() const;
    float getY() const;
    sf::Color getColor() const;
    sf::Vector2<float> getStartPos() const;
    sf::Vector2<float> getValue() const;
    sf::Vector2<float> getEnd() const;

    //Operators
    Vector2D operator *(float scalar) const;
    Vector2D operator +(const Vector2D& other) const;
    Vector2D operator -(const Vector2D& other) const;

    operator sf::Vector2<float>() const;
};


#endif //VECTOR_CALCULATOR_VECTOR2D_H