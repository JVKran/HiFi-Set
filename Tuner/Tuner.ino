#include "TEA5767.hpp"
#include "Graphics.hpp"
#include "Input.hpp"

TEA5767 radio = TEA5767(Wire, 5000);
Graphics display = Graphics(Wire, 128, 64);
Input input = Input(36, 39);

void setup(){
	Wire.begin();
	Serial.begin(9600);

	input.addListener(&radio);
	input.addListener(&display);
	radio.addListener(&display);

	display.begin();
	radio.begin();
}

void loop(){
	input();
	radio();
}
