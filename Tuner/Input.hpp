#ifndef __INPUT_HPP
#define __INPUT_HPP

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <EEPROM.h>
#include "Array.hpp"

class InputListener {
	public:
		virtual void frequencySelected(const float frequency) = 0;
};

class Input {
	private:
		ESP32Encoder encoder;
		int32_t lastPosition;
		bool frequencySaved = false;
		unsigned long long int lastFrequencyChange;

		array<InputListener*, 5> listeners;
		uint8_t amountOfListeners = 0;
	public:
		Input(const uint8_t dataOutPin, const uint8_t clockPin);
		void begin();

		void addListener(InputListener * listenerToAdd);
		void operator()();
};

#endif //__INPUT_HPP