#include <SFML/Graphics.hpp>
#include "emulator.h"

int main() {
	Emulator emu;
	emu.reset();

    sf::RenderWindow window(sf::VideoMode(768, 384), "Chasm");
	window.setFramerateLimit(60);
	window.clear(sf::Color(0, 45, 70));
	window.display();

	sf::RectangleShape pixel(sf::Vector2f(12, 12));
	pixel.setFillColor(sf::Color(25, 100, 150));

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		emu.cycle();

		if (emu.getDrawFlag()) {
			window.clear(sf::Color(0, 45, 70));

			bool* videoBuffer = emu.getVideoBuffer();
			for (int j = 0; j < 32; j++) {
				for (int i = 0; i < 64; i++) {
					if (videoBuffer[j * 64 + i]) {
						pixel.setPosition(i * 12.f, j * 12.f);
						window.draw(pixel);
					}
				}
			}

			window.display();
		}
    }
    return 0;
}