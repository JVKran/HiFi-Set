#include "Graphics.hpp"

Graphics::Graphics(TwoWire & bus, const uint8_t width, const uint8_t height):
	display(Adafruit_SSD1306(width, height, &bus, -1))
{}

void Graphics::begin(){
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("Check SSD1306 Wiring!");
	}
	display.clearDisplay();
	displayWifi(255);
}

void Graphics::displayTime(const String time){
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(2, 2);
	display.print(lastTime);
	display.setTextColor(WHITE);
	display.setCursor(2, 2);
	display.print(time);
	lastTime = time;
	display.display(); 
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

void Graphics::displaySetting(const String & setting){
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0,54);
	display.print(lastSetting);
	display.setTextColor(WHITE);
	display.setCursor(0,54);
	display.print(setting);
	lastSetting = setting;
	display.display();
}

void Graphics::displayStrength(const uint8_t & signalStrength, const uint8_t startX, const uint8_t startY){
	for(uint8_t i = 0; i < lastStrength / 50; i++){
		display.drawLine(startX + i * 2, startY, startX + i * 2, startY - i * 2, BLACK);
	}
	for(uint8_t i = 0; i < signalStrength / 50; i++){
		display.drawLine(startX + i * 2, startY, startX + i * 2, startY - i * 2, WHITE);
	}
	display.display();
}

void Graphics::displayStatistics(const bool stereo, const uint8_t signalStrength){
	display.setTextSize(0);
	display.setTextColor(BLACK);
	display.setCursor(117,0);
	if(lastStereo){
		display.print(2);
	} else {
		display.print(1);
	}
	display.setTextColor(WHITE);
	display.setCursor(117,0);
	if(stereo){
		display.print(2);
	} else {
		display.print(1);
	}
	displayStrength(signalStrength);
	lastStrength = signalStrength;
	lastStereo = stereo;
}