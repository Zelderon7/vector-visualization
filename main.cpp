#include <SFML/Graphics.hpp>
#include "models/Vector2D.h"
#include "models/VectorSpace.h"

std::vector<std::pair<sf::Vertex, sf::Vertex>> getRandomLines(sf::RenderWindow& window);

int prevFrame = -1;
int frame = 0;
const int numFrames = 5;
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

                if (key->code == sf::Keyboard::Key::Right) {
                    frame++;
                    frame %= (numFrames+1);
                }

                if (key->code == sf::Keyboard::Key::Left) {
                    frame--;
                    if (frame < 0) frame = 0;
                }
            }
        }

        if (prevFrame != frame) {
            RenderScreen(window);
            prevFrame = frame;
        }
    }

    return 0;
}

void RenderScreen(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    VectorSpace<Vector2D> space;

    Vector2D vec = Vector2D(3, 5, sf::Color::Red);

    Vector2D vec2 = Vector2D(1, 0, sf::Color::Yellow, &vec);

    Vector2D vec3 = vec + vec2;
    vec3.setColor(sf::Color::Blue);

    Vector2D vec4 = {1, 2.5, sf::Color::Red};
    vec4.setPrevious(&vec3);

    Vector2D vec5 = vec3 + vec4;
    vec5.setColor(sf::Color::White);

    space.add(&vec);
    space.add(&vec2);
    space.add(&vec3);
    space.add(&vec4);
    space.add(&vec5);

    space.draw(window);

    window.display();
}
