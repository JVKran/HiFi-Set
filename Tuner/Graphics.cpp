#include "Graphics.hpp"

void Graphics::begin(){
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
		Serial.println("Check SSD1306 Wiring!");
	}
	display.clearDisplay();
	//displayStatistics(true, 208);
}

void Graphics::displayFrequency(const float frequency){
	display.setTextSize(3);
	display.setTextColor(BLACK);
	display.setCursor(20,20);
	display.println(lastFrequency);
	display.setTextColor(WHITE);
	display.setCursor(20,20);
	display.println(frequency, 1);
	display.display(); 
	lastFrequency = frequency;
}

void Graphics::displayStatistics(const bool stereo, const uint8_t signalStrength){
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0,0);
	if(lastStereo){
		display.print("ST");
	} else {
		display.print("  ");
	}
	display.setCursor(100,0);
	display.println(lastStrength);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	if(stereo){
		display.print("ST");
	} else {
		display.print("  ");
	}
	display.setCursor(100,0);
	display.println(signalStrength);
	display.display();
	lastStrength = signalStrength;
	lastStereo = stereo;
}