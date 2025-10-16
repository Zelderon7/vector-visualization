#ifndef VECTOR_CALCULATOR_VECTORSPACE_H
#define VECTOR_CALCULATOR_VECTORSPACE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Vector2D.h"

// Helper function (not part of class)
inline sf::Vector2<float> flip_y(const sf::Vector2<float>* vec, const float window_height) {
    return {vec->x, window_height - vec->y};
}

template <typename T>
class VectorSpace {
private:
    std::vector<const T*> vectors;
    void find_bounds(float* minX, float* minY, float* maxX, float* maxY);

    static float find_scale(const sf::RenderWindow& window, const float* min_x, const float* min_y, const float* max_x, const float* max_y, sf::Vector2<float>* offset);

    static void draw_arrow_head(sf::RenderWindow& window, const Vector2D* vec, const sf::Vector2<float>* end);
    static void draw_line(sf::RenderWindow& window, sf::Color color, const sf::Vector2<float>* start, const sf::Vector2<float>* end);
    void draw_arrow(sf::RenderWindow& window, const T* vec, const sf::Vector2<float>& offset, float scale);

public:
    static constexpr float ARROW_SIZE = 15;

    VectorSpace();
    ~VectorSpace();
    void add(const T* vec);
    void remove(const T* vec);
    void draw(sf::RenderWindow& window);
    void draw_arrow(sf::RenderWindow& window, const T* vec);
};

// Template implementations must be in header
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

template<typename T>
float VectorSpace<T>::find_scale(const sf::RenderWindow &window, const float* min_x, const float* min_y, const float* max_x, const float* max_y,
    sf::Vector2<float>* offset) {

    const float scale = std::min(
        window.getSize().x / (*max_x - *min_x),
        window.getSize().y / (*max_y - *min_y));

    *offset = sf::Vector2(*min_x, *min_y) * -scale;
    return scale;
}

// Specializations for Vector2D
template<>
inline void VectorSpace<Vector2D>::find_bounds(float* minX, float* minY, float* maxX, float* maxY) {
    *minX = 0;
    *minY = 0;
    *maxX = -MAXFLOAT;
    *maxY = -MAXFLOAT;

    for (const auto vector : vectors) {
        const auto end = vector->getEnd();
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

template<>
inline void VectorSpace<Vector2D>::draw_line(sf::RenderWindow& window, const sf::Color color,
    const sf::Vector2<float>* start, const sf::Vector2<float>* end){

    const sf::Vertex lines[] = {
        sf::Vertex(flip_y(start, window.getSize().y), color),
        sf::Vertex(flip_y(end, window.getSize().y), color),
    };

    window.draw(lines, 2, sf::PrimitiveType::Lines);
}

template<>
inline void VectorSpace<Vector2D>::draw_arrow_head(sf::RenderWindow& window, const Vector2D* vec,
    const sf::Vector2<float>* end) {
    const sf::Vector2 norm = vec->getValue().normalized();
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

template<>
inline void VectorSpace<Vector2D>::draw_arrow(sf::RenderWindow &window, const Vector2D *vec, const sf::Vector2<float> &offset, float scale) {
    sf::Vector2<float> start = (vec->getStartPos() + offset) * scale;
    sf::Vector2<float> end = (vec->getEnd() + offset) * scale;

    draw_line(window, vec->getColor(), &start, &end);
    draw_arrow_head(window, vec, &end);
}

template<>
inline void VectorSpace<Vector2D>::draw_arrow(sf::RenderWindow& window, const Vector2D* vec) {
    float minX, maxX, minY, maxY;
    find_bounds(&minX, &minY, &maxX, &maxY);
    sf::Vector2<float> offset;
    const float scale = find_scale(window, &minX, &minY, &maxX, &maxY, &offset);

    draw_arrow(window, vec, offset, scale);
}

template<>
inline void VectorSpace<Vector2D>::draw(sf::RenderWindow& window) {
    float minX, maxX, minY, maxY;
    find_bounds(&minX, &minY, &maxX, &maxY);
    sf::Vector2<float> offset;
    const float scale = find_scale(window, &minX, &minY, &maxX, &maxY, &offset);

    for (const auto vector : vectors) {
        draw_arrow(window, vector, offset, scale);
    }
}

// Smth

#endif //VECTOR_CALCULATOR_VECTORSPACE_H