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

	switch(EEPROM.read(2)){
		case 0: {
			state = states::PRESETS;
			for(uint8_t i = 0; i < amountOfListeners; i++){
				listeners[i]->settingSelected("Presets");
			}
			break;
		}
		case 1: {
			state = states::INTERNET;
			for(uint8_t i = 0; i < amountOfListeners; i++){
				listeners[i]->settingSelected("Internet");
			}
			break;
		}
		default: {
			state = states::FREE;
			for(uint8_t i = 0; i < amountOfListeners; i++){
				listeners[i]->settingSelected("Free");
			}
			break;
		}
	}
	encoder.setCount(frequency * 10 - 869);
}

void Interface::addListener(InterfaceListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}

void Interface::handleSwitchPress(){
	if(encoder.getSwitch()){
		Serial.print("Pressed");
		switch(state){
			case states::FREE: {
				state = states::PRESETS;
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->settingSelected("Presets");
				}
				EEPROM.write(2, 0);
				break;
			}
			case states::PRESETS: {
				state = states::INTERNET;
				float desiredFrequency = frequencyPresets[encoder.getCount() % 8] + 0.1;
				encoder.setCount(float(desiredFrequency - 87) * 10);
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->settingSelected("Internet");
				}
				EEPROM.write(2, 1);
				break;
			}
			case states::INTERNET: {
				state = states::FREE;
				float desiredFrequency = frequencyPresets[encoder.getCount() % 8] + 0.1;
				encoder.setCount(float(desiredFrequency - 87) * 10);
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->settingSelected("Free");
				}
				EEPROM.write(2, 2);
				break;
			}
			default: {
				break;
			}
		}
		EEPROM.commit();
	}
}

void Interface::operator()(){
	handleSwitchPress();
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
			case states::PRESETS: {
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->frequencySelected(frequencyPresets[encoder.getCount() % 8]);
					listeners[i]->settingSelected(stations[encoder.getCount() % 8]);
				}
				break;
			}
			case states::INTERNET: {
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->hostSelected(hostnames[encoder.getCount() % 2]);
					listeners[i]->pathSelected(pathnames[encoder.getCount() % 2]);
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
			} else if (state == states::PRESETS){
				frequency = frequencyPresets[encoder.getCount() % 8];
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
