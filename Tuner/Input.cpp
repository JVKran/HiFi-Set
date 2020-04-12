#include "Input.hpp"

Input::Input(const uint8_t dataOutPin, const uint8_t clockPin)
{
	ESP32Encoder::useInternalWeakPullResistors = true;
	encoder.clearCount();
	encoder.attachHalfQuad(dataOutPin, clockPin);
}

void Input::addListener(InputListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}

void Input::operator()(){
	if(lastPosition != encoder.getCount()){
		lastPosition = encoder.getCount();
		for(uint8_t i = 0; i < amountOfListeners; i++){
			listeners[i]->frequencySelected(87 + (float(lastPosition) / 10));
		}
	}
}
