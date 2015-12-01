/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove DHT Series (11 - 22)
	ROLE : 	Le Petit Cool Temperature and Humidity Sensor
			Le Cool Stick Temperature and Humidity Sensor
	@author Maximilien Daman	01/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

/*	Conditional Inclusion - NOT WORKING
		#ifdef CoolStick_h
		#include "CoolStick.h"
		#elif defined(PetitCool_h)
		#include "PetitCool.h"
		#endif
*/
	#include <CoolStick.h>
	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////CONSTRUCTEURS///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
Cool_DHT::Cool_DHT()
{}

void Cool_DHT::begin(uint8_t ConnectPin)
{
	Pin = ConnectPin;
	TypeDetection();
	delay((SensorType==22)?DHTMaxFreq:0.5*DHTMaxFreq);
	readSensor();
	errorlevel();
	Temp = getTemperature();
	Hum = getTemperature();
}

void Cool_DHT::TypeDetection()
{
//Test de detection de modele:
	SensorType = 22;
	readSensor();
//Type de Capteur:
	SensorType = (ErrorLevel==2)?11:(ErrorLevel==3)?0:22;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////ERRORLEVEL///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint8_t Cool_DHT::errorlevel()
{
//Initialisation:
	uint8_t Test;
//Modele non pris en charge:
	Test = (SensorType == 0)?0:1;
//Test de fonctionnement:
	if(Test)	{	Test = (Temp == -1)?0:1;	}
//Niveau d'erreur:
	ErrorLevel = (Test)?0:1;
	return ErrorLevel;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////FONCTIONS DHT///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Cool_DHT::readSensor()
{
//Initialisation
	unsigned long 	CurrentTime;
	unsigned long 	MeasurementTime;
	uint16_t 		readData = 0;
	uint16_t 		Humidity = 0;
	uint16_t 		Temperature = 0;
	Temp = -1;
	Hum = -1;
//Echantillon de requete:
	//Signal d'envoi:
	digitalWrite(Pin, LOW);
	pinMode(Pin, OUTPUT);
	//Preparation du capteur:
	if(SensorType == 11) delay(18);
	else delayMicroseconds(800);
	pinMode(Pin, INPUT);
	//Mise en Mode Lecture:
	digitalWrite(Pin, HIGH);
//Lecture du Capteur
	for(int8_t i = -3; i < 2*40; i++)
	{
		CurrentTime = micros();
		do
		{
			MeasurementTime = micros()-CurrentTime;
			if (MeasurementTime > 90)
			{
				ErrorLevel = 2;
				return;
			}
		}
		while(digitalRead(Pin) == (i&1)?HIGH:LOW);		
		if(i>=0 && (i&1))
		{
			readData <<= 1;
			if (MeasurementTime > 30)
			{
				readData |= 1;
			}
		}
		switch(i)
		{
			case 31:
				Humidity = readData;
				break;
			case 63:
				Temperature = readData;
				readData = 0;
				break;
		}
	}
//Verification de fin de lecture:
	if((uint8_t)(((uint8_t)Humidity)+(Humidity>>8)+((uint8_t)Temperature)+(Temperature>>8)) != readData)
	{
		ErrorLevel = 3;
		return;
	}
//Enregistrement des donnees:
	if(SensorType == 11)
	{
		Hum = Humidity>>8;
		Temp = Temperature>>8;
	}
	if(SensorType == 22)
	{
		Hum = Humidity * 0.1;
		if(Temperature & 0x8000)	{	Temperature = -(int16_t)(Temperature & 0x7FFF);	}
		Temp = ((int16_t)Temperature) * 0.1;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////