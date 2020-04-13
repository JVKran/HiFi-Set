#ifndef __ROTARY_ENCODER_HPP
#define __ROTARY_ENCODER_HPP

#include <Arduino.h>
#include <ESP32Encoder.h>

class RotaryEncoder {
	private:
		const uint8_t switchPin;
		bool isButtonPressed;

		ESP32Encoder encoder;

	public:
		RotaryEncoder(const uint8_t dataOutPin, const uint8_t clockPin, const uint8_t switchPin);
		void begin();

		uint8_t getSwitchPin() const {
			return switchPin;
		}

		void buttonPressed(){
			isButtonPressed = true;
		}

		int32_t getCount(){
			return encoder.getCount();
		}

		bool getSwitch(){
			if(isButtonPressed){
				isButtonPressed = false;
				return true;
			} else {
				return false;
			}
		}

		void setCount(const int32_t & count){
			encoder.setCount(count);
		}
};

#endif //__ROTARY_ENCODER_HPP