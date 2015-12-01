/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Moisture Sensor
	ROLE : 	Le Petit Cool Moisture Sensor
			Le Cool Stick Moisture Sensor
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef CoolMoist_h
#define CoolMoist_h

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
	
	class CoolMoist
	{
		public:
			CoolMoist();
			void begin(uint8_t ConnectPin);
			uint8_t errorlevel();
			inline	void	setError(uint8_t err)	{	ErrorLevel = err;	}
		public:
			inline void 	readVal()	{	Val = int(map(analogRead(Pin), 0, 1023, 0 ,100));	}
			inline uint8_t	getVal()	{	return Val;	}
		private:
			uint8_t Pin;
			uint8_t ErrorLevel;
			uint8_t Val;
	};
#endif