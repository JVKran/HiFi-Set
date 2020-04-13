#ifndef __GUI_HPP
#define __GUI_HPP

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Interface.hpp"
#include "TEA5767.hpp"
#include "Time.hpp"

class Graphics : public InterfaceListener, public RadioStatsListener, public TimeListener {
	private:
		Adafruit_SSD1306 display;

		float lastFrequency = 0.0;
		void displayFrequency(const float frequency);

		bool lastStereo = false;
		uint8_t lastStrength = 0;
		void displayStatistics(const bool stereo, const uint8_t signalStrength);

		void displayStrength(const uint8_t & signalStrength, const uint8_t startX = 118, const uint8_t startY = 10);

		String lastTime = "";
		void displayTime(const String time);

		enum class states {FREE, PRESETS};
		states state = states::FREE;

		String lastSetting;
		void displaySetting(const String & setting){
			display.setTextSize(1);
			display.setTextColor(BLACK);
			display.setCursor(0,54);
			display.print(lastSetting);
			display.setTextColor(WHITE);
			display.setCursor(0,54);
			display.print(setting);
			lastSetting = setting;
			display.display();
		}

	public:
		Graphics(TwoWire & bus, const uint8_t width, const uint8_t height);
		void begin();

		virtual void frequencySelected(const float frequency) override {
			displayFrequency(frequency);
		}

		virtual void newStatistics(const bool stereo, const uint8_t signalStrength) override {
			displayStatistics(stereo, signalStrength);
		}

		virtual void newTime(const String time) override {
			displayTime(time);
		}

		virtual void settingSelected(const String & setting) override {
			displaySetting(setting);
		}

};

#endif //__GUI_HPP