#ifndef __TIME_HPP
#define __TIME_HPP

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Array.hpp"

class TimeListener {
	public:
		virtual void newTime(const String time) = 0;
};

class Time {
	private:
		NTPClient timeClient;

		String formattedDate;
		String timeStamp;

		unsigned long long int lastUpdate = 0;

		array<TimeListener*, 5> listeners;
		uint8_t amountOfListeners = 0;
	public:
		Time(NTPClient & timeClient):
			timeClient(timeClient)
		{}
		void begin();

		void operator()();
		void addListener(TimeListener * listenerToAdd);
};

#endif //__TIME_HPP