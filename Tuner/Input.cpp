#include "Input.hpp"

Input::Input(const uint8_t dataOutPin, const uint8_t clockPin){
	ESP32Encoder::useInternalWeakPullResistors = true;
	encoder.clearCount();
	encoder.attachHalfQuad(dataOutPin, clockPin);
}

void Input::begin(){
	float frequency = EEPROM.read(0) + float(EEPROM.read(1)) / 10;
	Serial.println(frequency);
	for(uint8_t i = 0; i < amountOfListeners; i++){
		listeners[i]->frequencySelected(frequency);
	}
	encoder.setCount(frequency * 10 - 869);
}

void Input::addListener(InputListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}

void Input::operator()(){
	if(lastPosition != encoder.getCount()){
		lastPosition = encoder.getCount();
		float frequency = 87 + (float(lastPosition) / 10);
		for(uint8_t i = 0; i < amountOfListeners; i++){
			listeners[i]->frequencySelected(frequency);
		}
		lastFrequencyChange = millis();
		frequencySaved = false;
	} else if (!frequencySaved){
		if(millis() - lastFrequencyChange > 5000){
			int frequency = float(87 + (float(lastPosition) / 10)) * 10;
			EEPROM.write(0, frequency / 10);	// 
			EEPROM.write(1, frequency - (frequency / 10) * 10);
			EEPROM.commit();
			Serial.print(EEPROM.read(0));
			Serial.print(", ");
			Serial.println(EEPROM.read(1));
			frequencySaved = true;
		}
	}
}
