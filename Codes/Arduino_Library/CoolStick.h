/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Cool Stick
	ROLE : Le Cool Stick Machine Intelligence
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef CoolStick_h
#define CoolStick_h

	#define BTBaudRate 38400
	#define DHTMaxFreq 2500

//Inclusion des librairies
	#include "Arduino.h"
	#include <string.h>
	#include <SoftwareSerial.h>
	
	#include <Cool_DHT.h>
	#include <CoolLum.h>
	#include <CoolTemp.h>
	#include <CoolMoist.h>
	
	template<class T> void myPrint(const T Data, SoftwareSerial& BlueTooth)
	{
		Serial.print(Data);
		if(BlueTooth.available() == 0) BlueTooth.print(Data);
		return;
	}
	
	template<class T> void myPrintln(const T Data, SoftwareSerial& BlueTooth)
	{
		Serial.println(Data);
		if(BlueTooth.available() == 0) BlueTooth.println(Data);
		return;
	}
	
	template<class T>	void myFlush(T& BlueTooth)
	{
		Serial.flush();
		if(BlueTooth.available() == 0) BlueTooth.flush();
		return;
	}
	
	class CoolStick
	{
		public:
			CoolStick();
			inline void attachBlueTooth(SoftwareSerial* BT)	{	BTModule = BT;	}
			void begin(uint8_t RX, uint8_t TX, uint8_t MoisturePin, uint8_t LumPin, uint8_t Temperature);
			void setupBTconnection(uint8_t RX, uint8_t TX);
			void beginLum(uint8_t RX, uint8_t TX, uint8_t Pin);
			void beginMoist(uint8_t RX, uint8_t TX, uint8_t Pin);
			void beginTemp(uint8_t RX, uint8_t TX, uint8_t Pin);
		public:
			void getTemperature();
			void getTemperatureR();
			void getHumidity();
			void getLuminosity();
			void getLuminosityR();
			void getLuminosityB();
			void getMoisture();
		public:
			void checkMessage();
			void treatMessage();
			void takeMeasure();
			void mountMessage();
			void sendMessage();
		private:
			SoftwareSerial*		BTModule;
			Cool_DHT 			Capteur_TH;
			CoolTemp			Capteur_T;
			CoolLum				Capteur_L;
			CoolMoist			Capteur_M;
		private:
			String				ReceivedBuffer;
			String				SendingBuffer;
			bool				IsDHTHere;
			int					MessageLevel;
			unsigned long		DHTTimer[2];
	};
#endif