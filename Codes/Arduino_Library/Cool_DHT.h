/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove DHT Series (11 - 22)
	ROLE : 	Le Petit Cool Temperature and Humidity Sensor
			Le Cool Stick Temperature and Humidity Sensor
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_DHT_h
#define Cool_DHT_h

	#include "Arduino.h"
/*	Conditional Inclusion - NOT WORKING
		#ifdef CoolStick_h
		#include "CoolStick.h"
		#elif defined(PetitCool_h)
		#include "PetitCool.h"
		#endif	
*/
	#include <CoolStick.h>
	
	class Cool_DHT
	{
		public:
			Cool_DHT();
			void	begin(uint8_t ConnectPin);
			void 	TypeDetection();
			uint8_t	errorlevel();
			inline	void	setError(uint8_t Error)	{	ErrorLevel = Error;	}
		public:
			void	readSensor();
			inline	uint8_t		getType()			{	return SensorType;	}
			inline	int16_t		getTemperature()	{	return Temp;	}
			inline	int8_t		getHumidity()		{	return Hum;	}
		private:
			uint8_t		Pin;
			uint8_t		SensorType;
			uint8_t		ErrorLevel;
			int16_t		Temp;
			int8_t		Hum;
	};
#endif