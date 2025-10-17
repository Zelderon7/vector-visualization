//
// Created by zelderon on 10/7/25.
//

#ifndef VECTOR_CALCULATOR_VECTOR2D_H
#define VECTOR_CALCULATOR_VECTOR2D_H
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

template<typename T, std::size_t N>
class VectorN {
private:
    std::array<T, N> data;
    sf::Color color;
    const VectorN* previous;

public:
    //Constructors
    VectorN(const std::array<T, N> &data, sf::Color color, const VectorN* previous)
        : data{data}, color{color}, previous{previous} {}

    VectorN(const std::array<T, N> &data, sf::Color color)
        :VectorN(data, color, nullptr) {}

    explicit VectorN(const std::array<T, N> &data)
        :VectorN(data, sf::Color::White){}

    VectorN()
        :VectorN(std::array<T, N>{}) {}

    //Getters and Setters
    T& operator[](std::size_t index) {
        if (index >= N) {
            throw std::out_of_range("Trying to access an invalid index: " + std::to_string(index));
        }
        return data[index];
    }
    const T& operator[](std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Trying to access an invalid index: " + std::to_string(index));
        }
        return data[index];
    }

    void setColor(sf::Color color){this->color = color;}
    void setPrevious(const VectorN* previous){this->previous = previous;}

    [[nodiscard]] sf::Color getColor() const {return color;}
    [[nodiscard]] const VectorN* getPrevious() const {return this->previous;}

    //Operators
    [[nodiscard]] VectorN operator *(float scalar) const;
    [[nodiscard]] VectorN operator +(const VectorN& other) const;
    [[nodiscard]] VectorN operator -(const VectorN& other) const;

    [[nodiscard]] T magnitude() const;
    [[nodiscard]] T dot(const VectorN& other) const;
    [[nodiscard]] VectorN normalized() const;

    [[nodiscard]] sf::Vector2<T> getStartPos() const requires(N == 2);
    [[nodiscard]] sf::Vector2<T> getEndPos() const requires(N == 2);

    [[nodiscard]] explicit operator sf::Vector2f() const requires(N == 2){ return {this->data[0], this->data[1]}; }
};

#include "VectorN.tpp"

#endif //VECTOR_CALCULATOR_VECTOR2D_H