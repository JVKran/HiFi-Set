#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "TEA5767.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Time.hpp"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid     = "KraanBast2.4";
const char* password = "Snip238!";

TEA5767 radio = TEA5767(Wire, 5000);
Graphics display = Graphics(Wire, 128, 64);
Input input = Input(36, 39);
Time timeKeeper = Time(timeClient);

void connect(){
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(50);
	}
}

void setup(){
	connect();

	Wire.begin();
	Serial.begin(9600);
	EEPROM.begin(2);

	input.addListener(&radio);
	input.addListener(&display);
	radio.addListener(&display);
	timeKeeper.addListener(&display);

	display.begin();
	timeKeeper.begin();
	radio.begin();
	input.begin();
}

void loop(){
	timeKeeper();
	input();
	radio();
}
