#ifndef VECTOR_CALCULATOR_VECTORSPACE_H
#define VECTOR_CALCULATOR_VECTORSPACE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "VectorN.h"

// Trait to detect VectorN<U, 2>
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

// Helper function (not part of class)
inline sf::Vector2<float> flip_y(const sf::Vector2<float>* vec, const float window_height) {
    return {vec->x, window_height - vec->y};
}

template <typename T>
class VectorSpace {
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

#include "VectorSpace_2.tpp"
#include  "VectorSpace_G.tpp"

#endif //VECTOR_CALCULATOR_VECTORSPACE_H