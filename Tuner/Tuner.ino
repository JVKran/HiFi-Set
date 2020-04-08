#include "RDA5807.hpp"

RDA5807 radio = RDA5807(Wire);

void setup(){
	Wire.begin();
	Serial.begin(9600);
	radio.begin();
	radio.setFrequency(100.7);
}

void loop(){
	if(radio.testCorrectFunctioning()){
		Serial.println("Succesful");
	} else {
		Serial.println("Failed");
	}
	delay(1000);
}