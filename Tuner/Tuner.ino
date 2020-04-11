#include "TEA5767.hpp"

TEA5767 radio = TEA5767(Wire);

void setup(){
	Wire.begin();
	Serial.begin(9600);
	radio.begin();
	radio.setFrequency(100.7);
}

void loop(){
	Serial.print("Tuned Frequency: ");
	Serial.print(radio.getFrequency());
	Serial.print(", Signal Strength: ");
	Serial.print(radio.signalStrength());
	Serial.print(", Stereo: ");
	Serial.print(radio.stereoReception());
	Serial.println(radio.isStereo());
	delay(5000);
}