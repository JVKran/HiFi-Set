#include "Radio.hpp"

Radio::Radio(WiFiClient & client):
	client(client)
{}

void Radio::operator()(){
	if(state == states::INTERNET){
		if (!client.connected() or pathChanged) {
	        Serial.println("Reconnecting...");
	        if (client.connect(host, httpPort)) {
	            client.print(String("GET ") + path + " HTTP/1.1\r\n" +
	                         "Host: " + host + "\r\n" +
	                         "Connection: close\r\n\r\n");
		        pathChanged = false;
	        }
	    }

	    if (client.available() > 0) {
	        uint8_t bytesread = client.read(mp3buff, 128);
	        internet.playChunk(mp3buff, bytesread);
	    }
	}
}