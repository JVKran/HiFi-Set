#include "Interface.hpp"

Interface::Interface(RotaryEncoder & encoder):
	encoder(encoder)
{}

void Interface::begin(){
	float frequency = EEPROM.read(0) + float(EEPROM.read(1)) / 10;
	if(frequency < 80){
		frequency = 100.7;
	}
	Serial.println(frequency);
	for(uint8_t i = 0; i < amountOfListeners; i++){
		listeners[i]->frequencySelected(frequency);
	}
	encoder.setCount(frequency * 10 - 869);
	if(state == states::FREE){
		for(uint8_t i = 0; i < amountOfListeners; i++){
			listeners[i]->settingSelected("Free");
		}
	} else {
		for(uint8_t i = 0; i < amountOfListeners; i++){
			listeners[i]->settingSelected("Presets");
		}
	}
}

void Interface::addListener(InterfaceListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}

void Interface::operator()(){
	if(encoder.getSwitch()){
		Serial.print("Pressed");
		if(state == states::FREE){
			state = states::PRESETS;
			for(uint8_t i = 0; i < amountOfListeners; i++){
				listeners[i]->settingSelected("Presets");
			}
		} else {
			state = states::FREE;
			float desiredFrequency = presets[encoder.getCount() % 8] + 0.1;
			encoder.setCount(float(desiredFrequency - 87) * 10);
			for(uint8_t i = 0; i < amountOfListeners; i++){
				listeners[i]->settingSelected("Free");
			}
		}
	}
	if(lastPosition != encoder.getCount()){
		lastPosition = encoder.getCount();
		switch(state){
			case states::FREE: {
				float frequency = 87 + (float(lastPosition) / 10);
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->frequencySelected(frequency);
				}
				break;
			}
			default: {
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->frequencySelected(presets[encoder.getCount() % 8]);
				}
				break;
			}
		}
		Serial.println(encoder.getCount());
		lastFrequencyChange = millis();
		frequencySaved = false;
	} else if (!frequencySaved){
		if(millis() - lastFrequencyChange > 5000){
			int frequency;
			if(state == states::FREE){
				frequency = float(87 + (float(lastPosition) / 10)) * 10;
			} else {
				frequency = presets[encoder.getCount() % 8];
			}
			EEPROM.write(0, frequency / 10);	// 
			EEPROM.write(1, frequency - (frequency / 10) * 10);
			EEPROM.commit();
			Serial.print(EEPROM.read(0));
			Serial.print('.');
			Serial.print(EEPROM.read(1));
			Serial.println(" stored in memory.");
			frequencySaved = true;
		}
	}
}
