/// @file

#ifndef __TEA5767_HPP
#define __TEA5767_HPP

#include <Arduino.h>
#include <Wire.h>
#include "Input.hpp"
#include "Array.hpp"

class RadioStatsListener {
	public:
		virtual void newStatistics(const bool stereo, const uint8_t signalStrength) = 0;
};

/// \brief
/// TEA5767 Interface
/// \details
/// This is an interface that simplifies the use of TEA5767 chips. Tuning, Muting
/// and setting of Frequencies are some of its abilities. It is compatible with the module
/// and breakout, though the breakout has hardwareproblems. It doesn't support searching
/// for channels that well.
/// 
///	Some supported operations are:
///		- Set/Get Frequency
///		- Set/Get Stereo
///		- Set/Get Mute
///		- Set/Get Volume
///		- Automatic Search
///		- Set/Get Stand By
///		- Set/Get Software Programmable Ports
///		- More to be seen in tests
/// 
class TEA5767 : public InputListener {
	private:
		unsigned long long int updatePeriod;
		unsigned long long int lastUpdate;
		bool lastStereo;
		uint8_t lastSignalStrength;

		array<RadioStatsListener*, 5> listeners;
		uint8_t amountOfListeners = 0;

		TwoWire & bus;
		uint8_t bandLimit;
		uint8_t address;

		// I2C Communication
		uint8_t data[5] = {0x30, 0x1F, 0x10, 0x10, 0xC0};
		uint8_t status[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
		void setData();
		void getStatus();

		//Calucaltion of PLL word
		int testHiLo(const float frequency);
		void setPLL(const float frequency, unsigned int hilo);
	public:
		TEA5767(TwoWire & bus, unsigned long long int updatePeriod, uint8_t bandLimit = 0, uint8_t address = 0x60, const bool autoBegin = false);
		void begin();

		//Basic Settings
		void setClockFrequency(const unsigned int frequency = 32);
		unsigned int getClockFrequency();

		void setBandLimit(const unsigned int limit = 1);
		unsigned int hasBandLimit();

		void standBy(const bool sleep = true);
		bool isStandBy();

		//Basic Tuning
		void setFrequency(const float frequency, const int hiLoForce);
		void setFrequency(const float frequency);
		float getFrequency();
		unsigned int getIntFrequency();
		bool highSide();

		//Audio Qualitiy
		void audioSettings(const bool SNC = false, const bool HCC = true, const bool SM = true);

		unsigned int signalStrength();

		void setMute(const bool mute = true);
		void setMute(const char side, const bool mute = true);
		bool isMuted(const char side = 'a');
		unsigned int getVolume();

		bool bassBoosted();

		void setStereo(const bool stereo = true);
		bool isStereo();			//Stereo Output Set
		bool stereoReception();	//Stereo Signal Received

		//Search Settings
		void seek(const unsigned int direction);

		void searchLoop(const unsigned int direction, const unsigned int qualityThreshold = 3);
		void searchLoop(const float startFrequency, const unsigned int direction, const unsigned int qualityThreshold);

		void singleSearch(const unsigned int direction, const unsigned int qualityThreshold = 3);
		void singleSearch(const float startFrequency, const unsigned int direction, const unsigned int qualityThreshold);

		void altSearch(const unsigned int direction, const unsigned int qualityThreshold = 3, const float distance = 0.4);
		void altSearch(const float startFrequency, const unsigned int direction, const unsigned int inQualityThreshold, const float distance);

		bool inSearch();

		//Software Programmable Ports
		void setPort(const bool portOne, const bool portTwo, const bool searchIndicator = false);
		bool getPort(const unsigned int port);

		bool testCorrectFunctioning();

		void addListener(RadioStatsListener * listenerToAdd){
			listeners[amountOfListeners++] = listenerToAdd;
		}

		virtual void frequencySelected(const float frequency) override {
			setFrequency(frequency);
		}

		void operator()(){
			if(millis() - lastUpdate > updatePeriod){
				lastUpdate = millis();
				bool currentStereo = stereoReception();
				uint8_t currentStrength = signalStrength();
				if(lastStereo != currentStereo || lastSignalStrength != currentStrength) {
					for(uint8_t i = 0; i < amountOfListeners; i++){
						listeners[i]->newStatistics(currentStereo, currentStrength);
					}
					lastStereo = currentStereo;
					lastSignalStrength = currentStrength;
				}
			}
		}
};

#endif //__TEA5767_HPP