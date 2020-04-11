#ifndef __INPUT_HPP
#define __INPUT_HPP

#include <Arduino.h>
#include "Array.hpp"

class InputListener {
	public:
		virtual void frequencySelected(const float frequency) = 0;
};

class Input {
	private:
		const uint8_t frequencyKnob;
		uint16_t lastAnalogReading;
		float lastFrequency;
		unsigned long long int lastMeasurement;

		array<InputListener*, 5> listeners;
		uint8_t amountOfListeners = 0;
	public:
		Input(const uint8_t frequencyKnob);

		void addListener(InputListener * listenerToAdd);
		void operator()();
};

#endif //__INPUT_HPP