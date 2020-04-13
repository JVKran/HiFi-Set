#include "RotaryEncoder.hpp"

RotaryEncoder::RotaryEncoder(const uint8_t dataOutPin, const uint8_t clockPin, const uint8_t switchPin):
	switchPin(switchPin)
{
	pinMode(switchPin, INPUT_PULLUP);

	ESP32Encoder::useInternalWeakPullResistors = true;
	encoder.clearCount();
	encoder.attachHalfQuad(dataOutPin, clockPin);
}