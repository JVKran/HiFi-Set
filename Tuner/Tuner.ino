#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "TEA5767.hpp"
#include "Graphics.hpp"
#include "Interface.hpp"
#include "Time.hpp"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid     = "KraanBast2.4";
const char* password = "Snip238!";

TEA5767 radio = TEA5767(Wire);
Graphics display = Graphics(Wire, 128, 64);
RotaryEncoder encoder = RotaryEncoder(36, 39, 32);
Interface interface = Interface(encoder);
Time timeKeeper = Time(timeClient);

void buttonPressed(){
	encoder.buttonPressed();
}

void connect(){
	WiFi.mode(WIFI_STA);
	WiFi.setHostname("Radio");
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(50);
	}
}

void setup(){
	attachInterrupt(encoder.getSwitchPin(), buttonPressed, FALLING);

	connect();

	Wire.setClock(400000);
	Wire.begin();
	Serial.begin(9600);
	EEPROM.begin(2);

	interface.addListener(&radio);
	interface.addListener(&display);
	radio.addListener(&display);
	timeKeeper.addListener(&display);

	display.begin();
	timeKeeper.begin();
	radio.begin();
	interface.begin();
}

void loop(){
	timeKeeper();
	interface();
	// delay(100);
	// Serial.println(WiFi.RSSI());
}
