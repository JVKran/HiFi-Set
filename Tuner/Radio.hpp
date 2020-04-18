#ifndef __RADIO_HPP
#define __RADIO_HPP

#include <VS1053.h>
#include <WiFi.h>
#include <TEA5767.h>
#include "Interface.hpp"
#include "Array.hpp"

class RadioStatsListener {
	public:
		virtual void newStatistics(const bool stereo, const uint8_t signalStrength) = 0;
};

class Radio : public InterfaceListener {
	private:
		TEA5767 fm = TEA5767(Wire);
		bool lastStereo;
		uint8_t lastSignalStrength;

		VS1053 internet = VS1053(5, 16, 4);
		WiFiClient & client;
		bool pathChanged;
		const char* host;
		const char* path;
		int httpPort = 80;
		uint8_t mp3buff[128];

		enum class states {INTERNET, FM};
		states state;

		array<RadioStatsListener*, 2> listeners;
		uint8_t amountOfListeners = 0;

	public:
		Radio(WiFiClient & client);
		void operator()();

		void begin(){
			fm.begin();
			internet.begin();

		    internet.switchToMp3Mode();
		    internet.setVolume(90);
		    internet.setBass(4);
		    internet.setTreble(0);
		}

		void addListener(RadioStatsListener* listenerToAdd){
			listeners[amountOfListeners++] = listenerToAdd;
		}

		virtual void settingSelected(const String & setting) override {
			if(setting == "Internet"){
				state = states::INTERNET;
				fm.setMute(true);
				internet.setVolume(90);
			} else {
				state = states::FM;
				fm.setMute(false);
				internet.setVolume(0);
			}
		}

		virtual void frequencySelected(const float frequency) override {
			if(state == states::FM){
				fm.setFrequency(frequency);
				delay(200);
				bool currentStereo = fm.stereoReception();
				uint8_t currentStrength = fm.signalStrength();
				if(lastStereo != currentStereo || lastSignalStrength != currentStrength) {
					for(uint8_t i = 0; i < amountOfListeners; i++){
						listeners[i]->newStatistics(!currentStereo, currentStrength);
					}
					lastStereo = currentStereo;
					lastSignalStrength = currentStrength;
				}
			}
		}

		virtual void hostSelected(const char* newHost) override {
			host = newHost;
		}

		virtual void pathSelected(const char* newPath) override {
			path = newPath;
			pathChanged = true;
		}
};

#endif //__RADIO_HPP