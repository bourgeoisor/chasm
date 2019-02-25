#include "sound.h"

const unsigned SAMPLES = 44100;
const unsigned SAMPLE_RATE = 44100;
const unsigned AMPLITUDE = 30000;

Sound::Sound() {
	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;
	const double increment = 440. / 44100;
	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sin(x*TWO_PI);
		x += increment;
	}

	buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE);
	sound.setBuffer(buffer);
	sound.setLoop(true);
}

void Sound::play() {
	if (!playing) {
		playing = true;
		sound.play();
	}
}

void Sound::stop() {
	if (playing) {
		playing = false;
		sound.stop();
	}
}