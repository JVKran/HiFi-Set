#include "Time.hpp"

void Time::begin(){
	timeClient.begin();
	timeClient.setTimeOffset(7200);
	while(!timeClient.update()) {
		timeClient.forceUpdate();
	}
	formattedDate = timeClient.getFormattedDate();
	int splitT = formattedDate.indexOf("T");
	timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-4);
	for(uint8_t i = 0; i < amountOfListeners; i++){
		listeners[i]->newTime(timeStamp);
	}
}

void Time::operator()(){
	if(millis() - lastUpdate > 60000){
		while(!timeClient.update()) {
			timeClient.forceUpdate();
		}
		formattedDate = timeClient.getFormattedDate();
		int splitT = formattedDate.indexOf("T");
		timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-4);
		for(uint8_t i = 0; i < amountOfListeners; i++){
			listeners[i]->newTime(timeStamp);
		}
		lastUpdate = millis();
	}
}

void Time::addListener(TimeListener * listenerToAdd){
	listeners[amountOfListeners++] = listenerToAdd;
}