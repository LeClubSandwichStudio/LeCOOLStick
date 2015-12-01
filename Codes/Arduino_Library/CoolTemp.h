/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Temperature Sensor
	ROLE : 	Le Cool Stick Temperature Sensor
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef CoolTemp_h
#define CoolTemp_h

//Inclusion des librairies
	#include "Arduino.h"
/*	Conditional Inclusion - NOT WORKING
		#ifdef CoolStick_h
		#include "CoolStick.h"
		#elif defined(PetitCool_h)
		#include "PetitCool.h"
		#endif
*/
	#include <CoolStick.h>
	#include <math.h>
	
	#define THERMISTOR_VAL 3975
	class CoolTemp
	{
		public:
			CoolTemp();
			void 	begin(uint8_t ConnectPin);
			uint8_t errorlevel();
			inline	void	setError(uint8_t Error)	{	ErrorLevel = Error;	}
		public:
			void 	readData();
			inline 	float	getTemperature()	{	return Temperature;	}
			inline 	float 	getResistance()	{	return Resistance;	}
		private:
			uint8_t	Pin;
			uint8_t ErrorLevel;
			float	Temperature;
			float	Resistance;
	};
#endif