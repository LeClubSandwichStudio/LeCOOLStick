/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Luminosity Sensor
	ROLE : 	Le Petit Cool Luminosity Sensor
			Le Cool Stick Luminosity Sensor
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef CoolLum_h
#define CoolLum_h

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
	
	class CoolLum
	{
		public:
			CoolLum();
			void 	begin(uint8_t ConnectPin);
			uint8_t errorlevel();
			inline	void	setError(uint8_t Error)	{	ErrorLevel = Error;	}
		public:
			inline	void 	readData()			{	LVal = analogRead(Pin);	}
			inline 	void 	readRVal()			{	RVal = (1023-analogRead(Pin))*10/analogRead(Pin);	}
			inline 	void 	readBrightness()	{	BVal = int(map(analogRead(Pin), 0,1023,0,100));	}
		public:
			inline 	int		getVal()	{	return LVal;	}
			inline 	float 	getRVal()	{	return RVal;	}
			inline 	int 	getBVal()	{	return BVal;	}
		private:
			uint8_t	Pin;
			uint8_t ErrorLevel;
			int		LVal;
			float	RVal;
			int		BVal;	
	};
#endif