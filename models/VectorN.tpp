//
// Created by zelderon on 10/7/25.
//

#include <cmath>
#include <SFML/Graphics.hpp>

template<typename T, std::size_t N>
VectorN<T, N> VectorN<T, N>::operator*(float scalar) const {
    for (auto i : this->data)
        i *= scalar;

    return *this;
}

template<typename T, std::size_t N>
VectorN<T, 3> VectorN<T, N>::operator*(const VectorN &other) const requires (N == 3) {
    VectorN<T, 3> result;
    result.data[0] = data[1] * other.data[2] - data[2] * other.data[1];
    result.data[1] = data[2] * other.data[0] - data[0] * other.data[2];
    result.data[2] = data[0] * other.data[1] - data[1] * other.data[0];
    return result;
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