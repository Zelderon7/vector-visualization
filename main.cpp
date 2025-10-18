#include <iostream>
#include <SFML/Graphics.hpp>
#include "models/VectorN.h"
#include "models/VectorSpace.h"

std::vector<std::pair<sf::Vertex, sf::Vertex>> getRandomLines(sf::RenderWindow& window);

float scale = 80;

void RenderScreen(sf::RenderWindow& window);
void Render3DScreen(sf::RenderWindow& window);
void Zad1(sf::RenderWindow& window);

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Vector Calculator");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const auto& key = event->getIf<sf::Event::KeyPressed>();

                Zad1(window);

                //RenderScreen(window);
            }
        }
    }

    return 0;
}

void TestFindNormal() {
    VectorN<float, 3> vec = VectorN<float, 3>({1, 1, 1});
    VectorN<float, 3> vec2 = VectorN<float, 3>({1, 2, 1});

    VectorSpace<VectorN<float, 3>> space;
    space.add(&vec);
    space.add(&vec2);

    auto normal = VectorSpace<VectorN<float, 3>>::get_plane_normal(&vec, &vec2);

    std::cout << std::format("Normal = [{}, {}, {}]", normal[0], normal[1], normal[2]) << std::endl;
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

void Render3DScreen(sf::RenderWindow& window) {
    VectorSpace<VectorN<float, 3>> space;

    VectorN<float, 3> vec = VectorN<float, 3>({1, 1, 1});
    VectorN<float, 3> vec2 = VectorN<float, 3>({1, 2, 1});

    space.add(&vec);
    space.add(&vec2);

    window.clear(sf::Color::Black);

    VectorN<float, 3> normal = VectorSpace<VectorN<float, 3>>::get_plane_normal(&vec, &vec2);
    VectorSpace<VectorN<float, 2>> projected;
    space.get_plane_as_vector_space(&normal, projected);

    projected.draw(window);

    window.display();
}

void Zad1(sf::RenderWindow& window) {
    //Given A =(2, -3, 0) B =(3, 0, 4) and C =(3, 2, 0) Find the type of ABC
    using V3 = VectorN<float, 3>;
    using V2 = VectorN<float, 2>;

    V3 A = V3({2, -3, 0});
    V3 B = V3({3, 0, 4});
    V3 C = V3({3, 2, 0});

    V3 BA = A - B;
    V3 BC = C - B;
    V3 CA = A - C;

    BA.setColor(sf::Color::Red);
    BC.setColor(sf::Color::Blue);
    CA.setColor(sf::Color::White);

    const std::vector<V3 *> triangle = {&BC, &BA, &CA};

    VectorSpace<V2> projected;
    V3 normal = VectorSpace<V3>::get_plane_normal(&BC, &CA);
    VectorSpace<V3>::get_plane_as_vector_space(&normal, triangle, projected);

    auto ca = projected[2];
    auto bc = projected[0];

    ca->setPrevious(bc);

    window.clear(sf::Color::Black);
    projected.draw(window);
    window.display();
}


