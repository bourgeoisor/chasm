#include <SFML/Window.hpp>
#include "input.h"

const sf::Keyboard::Key keys[16] = {
	sf::Keyboard::X,
	sf::Keyboard::Num1,
	sf::Keyboard::Num2,
	sf::Keyboard::Num3,
	sf::Keyboard::Q,
	sf::Keyboard::W,
	sf::Keyboard::E,
	sf::Keyboard::A,
	sf::Keyboard::S,
	sf::Keyboard::D,
	sf::Keyboard::Z,
	sf::Keyboard::C,
	sf::Keyboard::Num4,
	sf::Keyboard::R,
	sf::Keyboard::F,
	sf::Keyboard::V,
};

Input::Input() {
}

bool Input::isKeyPressed(unsigned char key) {
	return sf::Keyboard::isKeyPressed(keys[key]);
}