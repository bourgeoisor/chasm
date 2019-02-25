#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "sound.h"
#include "emulator.h"

std::vector<unsigned char> loadRom(std::string path);

int main() {
    sf::RenderWindow window(sf::VideoMode(768, 384), "Chasm");
	window.setFramerateLimit(60);
	window.clear(sf::Color(0, 45, 70));
	window.display();

	sf::RectangleShape pixel(sf::Vector2f(12, 12));
	pixel.setFillColor(sf::Color(25, 100, 150));

	Input input;
	Sound sound;

	Emulator emu(input);
	emu.reset();

	std::string path;
	std::cout << "Welcome to Chasm, a CHIP-8 emulator!" << std::endl;
	std::cout << "ROM to open: ";
	std::cin >> path;

	std::vector<unsigned char> buffer = loadRom(path);
	emu.load(buffer);

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

		if (emu.getSoundTimer() > 0)
			sound.play();
		else
			sound.stop();
    }
    return 0;
}

std::vector<unsigned char> loadRom(std::string path) {
	std::ifstream file(path, std::ios::binary);
	if (file.fail()) {
		perror(path.c_str());
	}

	file.seekg(0, std::ios::end);
	long fileSize = file.tellg();

	file.seekg(0, std::ios::beg);
	fileSize -= file.tellg();

	std::vector<unsigned char> buffer(fileSize);

	file.read((char *)&(buffer[0]), fileSize);
	file.close();

	return buffer;
}