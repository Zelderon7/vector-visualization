#ifndef VECTOR_CALCULATOR_VECTORSPACE_H
#define VECTOR_CALCULATOR_VECTORSPACE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "VectorN.h"

#pragma region Type-detection Traits

template<typename T>
struct is_vector2 : std::false_type {};

template<typename U>
struct is_vector2<VectorN<U, 2>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector2_v = is_vector2<T>::value;

// Trait to detect VectorN<U, 3>
template<typename T>
struct is_vector3 : std::false_type {};

template<typename U>
struct is_vector3<VectorN<U, 3>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector3_v = is_vector3<T>::value;

#pragma endregion

#pragma region Helper functions

inline sf::Vector2<float> flip_y(const sf::Vector2<float>* vec, const float window_height) {
    return {vec->x, window_height - vec->y};
}
#pragma endregion

#pragma mark
#pragma mark - Class Definition
//region Class Definition
template <typename T>
class VectorSpace {
#pragma region Private Members
private:
    std::vector<const T*> vectors;
    void find_bounds(float* minX, float* minY, float* maxX, float* maxY) requires is_vector2_v<T>;

    static float find_scale(const sf::RenderWindow& window, const float* min_x, const float* min_y, const float* max_x,
        const float* max_y, sf::Vector2<float>* offset) requires is_vector2_v<T>;

    static void draw_arrow_head(sf::RenderWindow& window, const T* vec,
        const sf::Vector2<float>* end) requires is_vector2_v<T>;

    static void draw_line(sf::RenderWindow& window, sf::Color color, const sf::Vector2<float>* start,
        const sf::Vector2<float>* end) requires is_vector2_v<T>;

    void draw_arrow(sf::RenderWindow& window, const T* vec, const sf::Vector2<float>& offset,
        float scale) requires is_vector2_v<T>;

#pragma endregion

#pragma region Public Members
public:
    static constexpr float ARROW_SIZE = 15;

    VectorSpace();
    ~VectorSpace();
    void add(const T* vec);
    void remove(const T* vec);
    void clear() {
        vectors.clear();
    }

    void draw(sf::RenderWindow& window) requires is_vector2_v<T>;
    void draw_arrow(sf::RenderWindow& window, const T* vec) requires is_vector2_v<T>;

    static T get_plane_normal(const T *v1, const T *v2) requires is_vector3_v<T>;

    static std::pair<T, T> get_plane_basis(const T *v1, const T *n)requires is_vector3_v<T>;

    void get_plane_as_vector_space(const T *n, VectorSpace<VectorN<typename T::value_type, 2>> &out_plane_space) const requires is_vector3_v<T> {


    }
#pragma endregion
};

//endregion

#pragma region Implementations


template<typename T>
VectorSpace<T>::VectorSpace() = default;

template<typename T>
VectorSpace<T>::~VectorSpace() {
    vectors.clear();
}

template<typename T>
void VectorSpace<T>::add(const T* vec) {
    vectors.push_back(vec);
}

template<typename T>
void VectorSpace<T>::remove(const T* vec) {
    auto it = std::find(vectors.begin(), vectors.end(), vec);
    if (it != vectors.end()) {
        vectors.erase(it);
    }
}

#pragma mark - 2D


template<typename T>
float VectorSpace<T>::find_scale(const sf::RenderWindow &window, const float* min_x, const float* min_y, const float* max_x, const float* max_y,
                                                 sf::Vector2<float>* offset) requires is_vector2_v<T> {

    const float scale = std::min(
        window.getSize().x / (*max_x - *min_x),
        window.getSize().y / (*max_y - *min_y));

    *offset = sf::Vector2(*min_x, *min_y) * -scale;
    return scale;
}

template<typename T>
inline void VectorSpace<T>::find_bounds(float* minX, float* minY, float* maxX, float* maxY) requires is_vector2_v<T> {

    *minX = 0;
    *minY = 0;
    *maxX = -MAXFLOAT;
    *maxY = -MAXFLOAT;

    for (const auto vector : vectors) {
        const auto end = vector->getEndPos();
        if (end.x > *maxX)
            *maxX = end.x;
        if (end.y > *maxY)
            *maxY = end.y;
        if (end.x < *minX)
            *minX = end.x;
        if (end.y < *minY)
            *minY = end.y;
    }
}

template<typename T>
inline void VectorSpace<T>::draw_line(sf::RenderWindow& window, const sf::Color color,
    const sf::Vector2<float>* start, const sf::Vector2<float>* end) requires is_vector2_v<T> {

    const sf::Vertex lines[] = {
        sf::Vertex(flip_y(start, window.getSize().y), color),
        sf::Vertex(flip_y(end, window.getSize().y), color),
    };

    window.draw(lines, 2, sf::PrimitiveType::Lines);
}

template<typename T>
void VectorSpace<T>::draw_arrow_head(sf::RenderWindow& window, const T* vec,
    const sf::Vector2<float>* end) requires is_vector2_v<T> {

    const sf::Vector2 norm = (static_cast<sf::Vector2<float>>(*vec)).normalized();
    const sf::Vector2 arrow_base = {norm.y, -norm.x};
    const sf::Vector2 base_center = *end - (norm * ARROW_SIZE);

    const auto left = base_center + arrow_base * (ARROW_SIZE / 2.0f);
    const auto right = base_center - arrow_base * (ARROW_SIZE / 2.0f);

    const sf::Vertex arrow[] = {
        sf::Vertex(flip_y(end, window.getSize().y), vec->getColor()),
        sf::Vertex(flip_y(&left, window.getSize().y), vec->getColor()),
        sf::Vertex(flip_y(&right, window.getSize().y), vec->getColor())
    };

    window.draw(arrow, 3, sf::PrimitiveType::Triangles);
}

template<typename T>
void VectorSpace<T>::draw_arrow(sf::RenderWindow &window, const T* vec, const sf::Vector2<float> &offset, float scale)
    requires is_vector2_v<T> {

    sf::Vector2<float> start = (vec->getStartPos() + offset) * scale;
    sf::Vector2<float> end = (vec->getEndPos() + offset) * scale;

    draw_line(window, vec->getColor(), &start, &end);
    draw_arrow_head(window, vec, &end);
}

template<typename T>
void VectorSpace<T>::draw_arrow(sf::RenderWindow& window, const T* vec) requires is_vector2_v<T> {
    float minX, maxX, minY, maxY;
    find_bounds(&minX, &minY, &maxX, &maxY);
    sf::Vector2<float> offset;
    const float scale = find_scale(window, &minX, &minY, &maxX, &maxY, &offset);

    draw_arrow(window, vec, offset, scale);
}

template<typename T>
inline void VectorSpace<T>::draw(sf::RenderWindow& window) requires is_vector2_v<T> {

    float minX, maxX, minY, maxY;
    find_bounds(&minX, &minY, &maxX, &maxY);
    sf::Vector2<float> offset;
    const float scale = find_scale(window, &minX, &minY, &maxX, &maxY, &offset);

    for (const auto vector : vectors) {
        draw_arrow(window, vector, offset, scale);
    }
}

#pragma mark - 3D

template<typename T>
T VectorSpace<T>::get_plane_normal(const T *v1, const T *v2) requires is_vector3_v<T> {
    return (*v1 * *v2).normalized();
}

template<typename T>
std::pair<T, T> VectorSpace<T>::get_plane_basis(const T *v1, const T *n) requires is_vector3_v<T> {
    T u = *v1;
    u.normalized();

    T v = n * u;
    v.normalized();

    return {u, v};
}

#pragma endregion

#include "VectorSpace_2.tpp"
#include  "VectorSpace_G.tpp"

#endif //VECTOR_CALCULATOR_VECTORSPACE_H