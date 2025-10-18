//
// Created by zelderon on 10/7/25.
//

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

#pragma region Definitions
template<typename T, std::size_t N>
class VectorN {
private:
#pragma mark - Private
    std::array<T, N> data;
    sf::Color color;
    const VectorN* previous;

#pragma mark - Public
public:
    using value_type = T;

#pragma mark - Constructors
    //Constructors
    VectorN(const std::array<T, N> &data, sf::Color color, const VectorN* previous)
        : data{data}, color{color}, previous{previous} {}

    VectorN(const std::array<T, N> &data, sf::Color color)
        :VectorN(data, color, nullptr) {}

    explicit VectorN(const std::array<T, N> &data)
        :VectorN(data, sf::Color::White){}

    VectorN()
        :VectorN(std::array<T, N>{}) {}

#pragma mark - Getters and Setters
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

#pragma mark - Operators
    //Operators
    [[nodiscard]] VectorN operator *(float scalar) const;
    // Cross product only available for 3D vectors
    [[nodiscard]] VectorN<T, 3> operator*(const VectorN& other) const requires (N == 3);
    [[nodiscard]] VectorN operator +(const VectorN& other) const;
    [[nodiscard]] VectorN operator -(const VectorN& other) const;
    [[nodiscard]] explicit operator sf::Vector2f() const requires(N == 2){ return {this->data[0], this->data[1]}; }


#pragma mark - Vector Math

    [[nodiscard]] bool is_zero() const {
        for (std::size_t i = 0; i < N; ++i) {
            if (data[i] != 0)
                return false;
        }
        return true;
    }
    [[nodiscard]] T magnitude() const;
    [[nodiscard]] T dot(const VectorN& other) const;
    [[nodiscard]] VectorN normalized() const;

    [[nodiscard]] sf::Vector2<T> getStartPos() const requires(N == 2);
    [[nodiscard]] sf::Vector2<T> getEndPos() const requires(N == 2);

    [[nodiscard]] bool is_in_plane(const VectorN *n, T epsilon = 1e-6) const requires(N==3);

    [[nodiscard]] VectorN<T, 2> get_plane_coordinates(const VectorN* b1, const VectorN* b2, const VectorN *n) const requires(N==3);



};
#pragma endregion

#pragma region Implementations

#pragma mark - Generic

template<typename T, std::size_t N>
VectorN<T, N> VectorN<T, N>::operator*(float scalar) const {
    for (auto i : this->data)
        i *= scalar;

    return *this;
}


template<typename T, std::size_t N>
VectorN<T, N> VectorN<T, N>::operator+(const VectorN &other) const {

    std::array<T, N> res;
    for (int i = 0; i < N; i++)
        res[i] = data[i] + other.data[i];

    return VectorN<T, N>(res);
}

template<typename T, std::size_t N>
VectorN<T, N> VectorN<T, N>::operator-(const VectorN &other) const {

    T res[N];
    for (int i = 0; i < N; i++)
        res[i] = data[i] - other.data[i];

    return VectorN<T, N>(&res);
}

template<typename T, std::size_t N>
T VectorN<T, N>::magnitude() const {
    T sum = T();
    for (std::size_t i = 0; i < N; i++)
        sum += this->data[i] * this->data[i];

    return sqrt(sum);
}

template<typename T, std::size_t N>
T VectorN<T, N>::dot(const VectorN &other) const {

    T result = T(0);
    for (int i = 0; i < N; i++) {
        result += this->data[i] * other.data[i];
    }

    return result;
}

template<typename T, std::size_t N>
VectorN<T, N> VectorN<T, N>::normalized() const {
    T magnitude = this->magnitude();
    if (magnitude == 0) [[unlikely]]{
        return *this;
    }

    VectorN<T, N> result;
    for (std::size_t i = 0; i < N; i++)
        result[i] = this->data[i] / magnitude;

    return result;
}

#pragma mark - 2D

template<typename T, std::size_t N>
sf::Vector2<T> VectorN<T, N>::getStartPos() const requires (N == 2) {
    return previous ?
        previous->getEndPos() :
        sf::Vector2<T>(0, 0);
}

template<typename T, std::size_t N>
sf::Vector2<T> VectorN<T, N>::getEndPos() const requires (N == 2) {
    return getStartPos() + static_cast<sf::Vector2<T>>(*this);
}

#pragma mark - 3D

template<typename T, std::size_t N>
VectorN<T, 3> VectorN<T, N>::operator*(const VectorN &other) const requires (N == 3) {
    VectorN<T, 3> result;
    result.data[0] = data[1] * other.data[2] - data[2] * other.data[1];
    result.data[1] = data[2] * other.data[0] - data[0] * other.data[2];
    result.data[2] = data[0] * other.data[1] - data[1] * other.data[0];
    return result;
}

template<typename T, std::size_t N>
bool VectorN<T, N>::is_in_plane(const VectorN *n, T epsilon) const requires(N == 3){
    auto difference = this->dot(n);

    return difference <= epsilon;
}

template<typename T, std::size_t N>
VectorN<T, 2> VectorN<T, N>::get_plane_coordinates(const VectorN* b1, const VectorN* b2, const VectorN *n) const requires(N==3)
{
    if (!this->is_in_plane(n)) {
        throw std::invalid_argument("Vector is not in the plane defined by n");
    }

    auto alpha = this->dot(*b1);
    auto beta = this->dot(*b2);

    return VectorN<T, 2>(alpha, beta);
}

#pragma endregion
