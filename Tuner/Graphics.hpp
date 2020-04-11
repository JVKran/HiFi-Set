#ifndef __GUI_HPP
#define __GUI_HPP

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Input.hpp"
#include "TEA5767.hpp"

class Graphics : public InputListener, public RadioStatsListener {
	private:
		Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

		void displayFrequency(const float frequency);
		float lastFrequency = 0.0;

		void displayStatistics(const bool stereo, const uint8_t signalStrength);
		bool lastStereo = false;
		uint8_t lastStrength = 0;

	public:
		void begin();

		virtual void frequencySelected(const float frequency) override {
			displayFrequency(frequency);
		}

		virtual void newStatistics(const bool stereo, const uint8_t signalStrength) override {
			displayStatistics(stereo, signalStrength);
		}
};

#endif //__GUI_HPP