#pragma once

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