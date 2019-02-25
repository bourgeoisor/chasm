#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>

class Sound {
public:
	Sound();
	void play();
	void stop();
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	bool playing;
};

#endif