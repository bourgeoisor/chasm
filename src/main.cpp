#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include "emulator.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(768, 384), "Chasm");
	window.setFramerateLimit(60);
	window.clear(sf::Color(0, 45, 70));
	window.display();

	sf::RectangleShape pixel(sf::Vector2f(12, 12));
	pixel.setFillColor(sf::Color(25, 100, 150));

	Input input;

	Emulator emu(input);
	emu.reset();

	const unsigned SAMPLES = 44100;
	const unsigned SAMPLE_RATE = 44100;
	const unsigned AMPLITUDE = 30000;

	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;
	const double increment = 440. / 44100;
	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sin(x*TWO_PI);
		x += increment;
	}

	sf::SoundBuffer Buffer;
	Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE);

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.setLoop(true);


	std::string filePath = "roms/PONG2";
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
	}
	file.seekg(0, std::ios::end);
	long fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	fileSize -= file.tellg();
	std::vector<unsigned char> buffer(fileSize);
	file.read((char *)&(buffer[0]), fileSize);
	file.close();

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
			Sound.play();
		else
			Sound.stop();
    }
    return 0;
}