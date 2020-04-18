#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "VS1053.hpp"
#include "TEA5767.hpp"
#include "Graphics.hpp"
#include "Interface.hpp"
// #include "Time.hpp"

// const char *host = "icecast-qmusicnl-cdp.triple-it.nl";
const char *host = "icecast.omroep.nl";
// const char *path = "/Qmusic_nl_live_96.mp3";
const char *path = "/radio1-bb-mp3";
int httpPort = 80;
uint8_t mp3buff[128];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid     = "KraanBast2.4";
const char* password = "Snip238!";
WiFiClient client;

TEA5767 radio = TEA5767(Wire);
Graphics display = Graphics(Wire, 128, 64);
RotaryEncoder encoder = RotaryEncoder(36, 39, 32);
Interface interface = Interface(encoder);
Time timeKeeper = Time(timeClient);
VS1053 player = VS1053(5, 16, 4);

int lastUpdate = 0;

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

	Serial.println("Connected!");
}

void setup(){
	attachInterrupt(encoder.getSwitchPin(), buttonPressed, FALLING);

	connect();

	Wire.setClock(400000);
	Wire.begin();
	SPI.begin();
	Serial.begin(9600);
	EEPROM.begin(3);
	player.begin();

    player.switchToMp3Mode();
    player.setVolume(90);
    player.setBass(4);
    player.setTreble(0);

	interface.addListener(&radio);
	interface.addListener(&display);
	radio.addListener(&display);
	timeKeeper.addListener(&display);

	display.begin();
	timeKeeper.begin();
	radio.begin();
	interface.begin();

	client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

	Serial.println(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
}

void loop(){
	timeKeeper();
	interface();
	// delay(100);
	// Serial.println(WiFi.RSSI());

	if (!client.connected()) {
        Serial.println("Reconnecting...");
        if (client.connect(host, httpPort)) {
            client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
            Serial.println(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
        }
    }

    if (client.available() > 0) {
        uint8_t bytesread = client.read(mp3buff, 128);
        player.playChunk(mp3buff, bytesread);
    }

    if(millis() - lastUpdate > 1000){
    	lastUpdate = millis();
    	Serial.println(player.getSeconds());
    }

}
