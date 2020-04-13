#ifndef __INPUT_HPP
#define __INPUT_HPP

#include <Arduino.h>
#include <EEPROM.h>
#include "RotaryEncoder.hpp"
#include "Array.hpp"

class InterfaceListener {
	public:
		virtual void frequencySelected(const float frequency) = 0;
		virtual void settingSelected(const String & setting){}
};

class Interface {
	private:
		RotaryEncoder & encoder;
		int32_t lastPosition = 0;
		bool frequencySaved = false;
		unsigned long long int lastFrequencyChange;

		array<InterfaceListener*, 5> listeners;
		uint8_t amountOfListeners = 0;

		enum class states {FREE, PRESETS};
		states state = states::PRESETS;
		array<float, 8> presets { {92.6, 96.8, 98.9, 100.1, 100.7, 101.2, 102.7, 107.5} };
		array<const char*, 8> stations { {"NPO R2", "NPO R3", "NPO R1", "BNR NIEUWS", "QMUSIC", "SKYRADIO", "RADIO538", "MIDLAND"} };
	public:
		Interface(RotaryEncoder & encoder);
		void begin();

		void addListener(InterfaceListener * listenerToAdd);
		void operator()();
};

#endif //__INPUT_HPP