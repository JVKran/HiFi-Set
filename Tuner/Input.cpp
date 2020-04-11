#include "Input.hpp"

Input::Input(const uint8_t frequencyKnob):
	frequencyKnob(frequencyKnob)
{
	pinMode(frequencyKnob, INPUT);
}

void Input::addListener(InputListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}

void Input::operator()(){
	if(millis() - lastMeasurement > 500){
		uint16_t analogReading = analogRead(frequencyKnob);
		if(analogReading != lastAnalogReading){
			float frequency = float(float(analogReading * 0.021) + 87);
			lastAnalogReading = analogReading;
			frequency = ((float)((int)(frequency * 10))) / 10;
			if(frequency != lastFrequency){
				lastFrequency = frequency;
				for(uint8_t i = 0; i < amountOfListeners; i++){
					listeners[i]->frequencySelected(frequency);
				}
			}
		}
		lastMeasurement = millis();
	}
}
