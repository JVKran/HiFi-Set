#ifndef __GUI_HPP
#define __GUI_HPP

#include <Arduino.h>
#include "Radio.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Interface.hpp"
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
		void displaySetting(const String & setting);

		uint8_t lastWifi;
		void displayWifi(const uint8_t signalStrength, const uint8_t startX = 108, const uint8_t startY = 10){
			display.drawPixel(startX, startY - 1, WHITE);
			display.drawCircle(startX, startY, 4, WHITE);
			display.drawCircle(startX, startY, 6, WHITE);
			display.drawCircle(startX, startY, 8, WHITE);
			display.drawCircle(startX, startY, 10, WHITE);
			display.fillTriangle(startX, startY, startX + 10, startY - 10, startX + 10, startY, BLACK);
			display.fillTriangle(startX, startY, startX - 10, startY - 10, startX - 10, startY, BLACK);
			display.fillRect(startX - 10, startY, 21, 11, BLACK);
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