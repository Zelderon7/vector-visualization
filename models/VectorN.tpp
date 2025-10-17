//
// Created by zelderon on 10/7/25.
//

#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

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

    return std::sqrt(sum);
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

