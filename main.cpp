#include <SFML/Graphics.hpp>
#include "models/VectorN.h"
#include "models/VectorSpace.h"

std::vector<std::pair<sf::Vertex, sf::Vertex>> getRandomLines(sf::RenderWindow& window);

float scale = 80;

void RenderScreen(sf::RenderWindow& window);

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Vector Calculator");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const auto& key = event->getIf<sf::Event::KeyPressed>();

            }
        }
    }

    return 0;
}

void RenderScreen(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    VectorSpace<VectorN<float, 2>> space;

    VectorN vec = VectorN<float, 2>({3, 5}, sf::Color::Red);

    VectorN vec2 = VectorN({1, 0}, sf::Color::Yellow, &vec);

    VectorN vec3 = vec + vec2;
    vec3.setColor(sf::Color::Blue);

    VectorN<float, 2> vec4 = {{1, 2.5}, sf::Color::Red};
    vec4.setPrevious(&vec3);

    VectorN vec5 = vec3 + vec4;
    vec5.setColor(sf::Color::White);

    space.add(&vec);
    space.add(&vec2);
    space.add(&vec3);
    space.add(&vec4);
    space.add(&vec5);

    space.draw(window);

    window.display();
}
