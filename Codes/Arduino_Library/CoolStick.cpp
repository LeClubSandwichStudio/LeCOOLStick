/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Cool Stick
	ROLE : Le Cool Stick Machine Intelligence
	@author Maximilien Daman	09/12/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

//Inclusion des librairies
	#include "Arduino.h"
	#include <string.h>
	#include <SoftwareSerial.h>
	
	#include "CoolStick.h"
	#include <Cool_DHT.h>
	#include <CoolLum.h>
	#include <CoolTemp.h>
	#include <CoolMoist.h>
	
static bool	IsBTSync = false;
static String myRead(SoftwareSerial& BlueTooth)
{
	String Res;
	Res.reserve(32);
	Res  = "";
	char recvChar;
	if(Serial.available()) Res = Serial.readStringUntil(10);
	else if(BlueTooth.available())
	{	
		while((BlueTooth.available() > 0)&&(recvChar != '\n'))
		{
			recvChar = BlueTooth.read();
			if(recvChar != '\n') Res.concat(recvChar);
		}
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////CONSTRUCTEURS///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
CoolStick::CoolStick()
{}

void CoolStick::begin(uint8_t RxD, uint8_t TxD, uint8_t MoisturePin, uint8_t LumPin, uint8_t TempPin)
{
//Initialisation:	
	//Générale:
	ReceivedBuffer.reserve(32);
	SendingBuffer.reserve(512);
	ReceivedBuffer = "";
	SendingBuffer = "";
	DHTTimer[0] = 0;
	DHTTimer[1] = millis();
	//Communication:
	if(!IsBTSync) setupBTconnection(RxD, TxD);
	myPrintln(F("\n---Initialisation Capteurs---"), *BTModule);
//Hydrometrie:
	Capteur_M.begin(MoisturePin);
	myPrintln(F("Moisture Checked"), *BTModule);
//Luminosité:	
	Capteur_L.begin(LumPin);
	myPrintln(F("Luminosity Checked"), *BTModule);
//Température:	
	if(TempPin > 14)	//Thermistance (Analogique)
	{
		IsDHTHere = false;
		Capteur_T.begin(TempPin);
		myPrintln(F("Temperature Checked"), *BTModule);
	}
	else				//DHT
	{
		IsDHTHere = true;
		Capteur_TH.begin(TempPin);
		DHTTimer[0] = DHTTimer[1];
		myPrint(F("DHT Checked : Type -> "), *BTModule);
		myPrintln(Capteur_TH.getType(), *BTModule);
	}
	myPrintln(F("\t----------\n"), *BTModule);
//Prompt:
	myPrintln(F("\nCoolStick peut vous fournir les données suivantes :\n\t->Hydratation du sol (Hydro)\n\t->Luminosite(Lum)\n\t->Temperature (Temp)"), *BTModule);
	if(IsDHTHere) myPrintln(F("\t->Humidite Relative (Hum)"), *BTModule);
	myPrintln(F("\nQuel parametre voulez vous connaitre?"), *BTModule);
}

void CoolStick::beginLum(uint8_t RxD, uint8_t TxD, uint8_t LumPin)
{
	//Initialisation:	
	//Générale:
	ReceivedBuffer.reserve(32);
	SendingBuffer.reserve(512);
	ReceivedBuffer = "";
	SendingBuffer = "";
	//Communication:
	if(!IsBTSync) setupBTconnection(RxD, TxD);
	myPrintln(F("\n---Initialisation Capteurs---"), *BTModule);
//Luminosité:	
	Capteur_L.begin(LumPin);
	myPrintln(F("Luminosity Checked"), *BTModule);
	myPrintln(F("\t----------\n"), *BTModule);
}

void CoolStick::beginMoist(uint8_t RxD, uint8_t TxD, uint8_t MoisturePin)
{
	//Initialisation:	
	//Générale:
	ReceivedBuffer.reserve(32);
	SendingBuffer.reserve(512);
	ReceivedBuffer = "";
	SendingBuffer = "";
	//Communication:
	if(!IsBTSync) setupBTconnection(RxD, TxD);
	myPrintln(F("\n---Initialisation Capteurs---"), *BTModule);
//Hydrometrie:
	Capteur_M.begin(MoisturePin);
	myPrintln(F("Moisture Checked"), *BTModule);
	myPrintln(F("\t----------\n"), *BTModule);
}

void CoolStick::beginTemp(uint8_t RxD, uint8_t TxD, uint8_t TempPin)
{
	//Initialisation:	
	//Générale:
	ReceivedBuffer.reserve(32);
	SendingBuffer.reserve(512);
	ReceivedBuffer = "";
	SendingBuffer = "";
	DHTTimer[0] = 0;
	DHTTimer[1] = millis();
	//Communication:
	if(!IsBTSync) setupBTconnection(RxD, TxD);
	myPrintln(F("\n---Initialisation Capteurs---"), *BTModule);
//Température:	
	if(TempPin > 14)	//Thermistance (Analogique)
	{
		IsDHTHere = false;
		Capteur_T.begin(TempPin);
		myPrintln(F("Temperature Checked"), *BTModule);
	}
	else				//DHT
	{
		IsDHTHere = true;
		Capteur_TH.begin(TempPin);
		DHTTimer[0] = DHTTimer[1];
		myPrint(F("DHT Checked : Type -> "), *BTModule);
		myPrintln(Capteur_TH.getType(), *BTModule);
	}
	myPrintln(F("\t----------\n"), *BTModule);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////ERRORLEVEL///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////COMMUNICATION/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CoolStick::setupBTconnection(uint8_t RxD, uint8_t TxD)
{
	IsBTSync = true;
	if(RxD == TxD)
	{
		Serial.print(F("Initialisation CoolStick sans Bluetooth\n"));
		return;
	}	
	else
	{
		Serial.print(F("Initialisation CoolStick\n"));
		pinMode(RxD, INPUT);
		pinMode(TxD, OUTPUT);
		(*BTModule).begin(BTBaudRate);	
	/* ST MODE:	*/
	#ifdef _STMode
		//Parametres generaux:
		String StringBuffer1_0, StringBuffer1_1;
		StringBuffer1_0.reserve(64);
		StringBuffer1_1.reserve(20);
		StringBuffer1_1 = "\r\n+INQ=1\r\n";
		StringBuffer1_0 = "\r\n+STWMOD=0\r\n";
		(*BTModule).print(StringBuffer1_0);
		delay(1000);
		StringBuffer1_0 = "\r\nSTBD=";
		StringBuffer1_0.concat(BTBaudRate);
		StringBuffer1_0.concat("\r\n");
		(*BTModule).print(StringBuffer1_0);
		delay(1000);
		StringBuffer1_0 = "\r\n+STNA=";
		StringBuffer1_0.concat(BTName);
		StringBuffer1_0.concat("\r\n");
		(*BTModule).print(StringBuffer1_0);
		delay(1000);
		StringBuffer1_0 = "\r\n+STPIN=";
		StringBuffer1_0.concat(BTPinCode);
		StringBuffer1_0.concat("\r\n");
		(*BTModule).print(StringBuffer1_0);
		delay(1000);
		StringBuffer1_0 = "\r\n+STOAUT=1\r\n";
		(*BTModule).print(StringBuffer1_0);
		delay(1000);
		StringBuffer1_0 = "\r\n+STAUTO=1\r\n";
		(*BTModule).print(StringBuffer1_0);
	//Mise en Fonctionnement:
		delay(2000);
		(*BTModule).println(StringBuffer1_1);
		delay(2000);
		(*BTModule).println(F("\tConnexion BlueTooth engagee."));
		(*BTModule).flush();
		delay(1000);
	#endif	
	/* AT MODE: */
	#ifdef _ATMode
		String StringBuffer1_0;
		StringBuffer1_0.reserve(64);
		StringBuffer1_0 = "AT";
		(*BTModule).print(StringBuffer1_0);
		delay(2000);
		//Set Baudrate:
		StringBuffer1_0.concat("+BAUD");
		StringBuffer1_0.concat(ATBaudRate);
		(*BTModule).print(StringBuffer1_0);
		StringBuffer1_0 = "";
		delay(2000);
		//Set Name:
		StringBuffer1_0 = "AT+NAME";
		StringBuffer1_0.concat(BTName);
		(*BTModule).print(StringBuffer1_0);
		StringBuffer1_0 = "";
		delay(2000);
		//Set Pin:
		StringBuffer1_0 = "AT+PIN";
		StringBuffer1_0.concat(BTPinCode);
		(*BTModule).print(StringBuffer1_0);
		delay(2000);
		//Sortie de mode AT:
		(*BTModule).print("AT+RESET");
		delay(2000);
		(*BTModule).println(F("\tConnexion BlueTooth engagee."));
		(*BTModule).flush();
	#endif
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////FONCTION DIY//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CoolStick::getTemperature()
{
//Mesure:
	if(!IsDHTHere) Capteur_T.readData();
	else
	{
		DHTTimer[1] = millis();
		if(DHTTimer[1] - DHTTimer[0] > DHTMaxFreq)
		{
			DHTTimer[0] = DHTTimer[1];
			Capteur_TH.readSensor();
		}
	}
//Ecriture:
	if(IsDHTHere)
	{
		myPrint("Temperature :", *BTModule);
		myPrint(Capteur_TH.getTemperature(), *BTModule);
		myPrint(" °C\n", *BTModule);
	}
	else
	{
		myPrint("Temperature :", *BTModule);
		myPrint(Capteur_T.getTemperature(), *BTModule);
		myPrint(" °C\n", *BTModule);
	}
	
}

void CoolStick::getTemperatureR()
{
	if(!IsDHTHere)
	{
	//Mesure:	
		Capteur_T.readData();
	//Ecriture:
		myPrint("Thermistance : \n\tResistance mesurée : ", *BTModule);
		myPrint(Capteur_T.getResistance(), *BTModule);
		myPrint(" Ohms\n", *BTModule);
		return;
	}
	else return;
	
}

void CoolStick::getHumidity()
{
//Mesure:
	if(IsDHTHere)
	{
		DHTTimer[1] = millis();
		if(DHTTimer[1] - DHTTimer[0] > DHTMaxFreq)
		{
			DHTTimer[0] = DHTTimer[1];
			Capteur_TH.readSensor();
		}
	}
//Ecriture:	
	if(IsDHTHere)
	{
		myPrint("Humidite :", *BTModule);
		myPrint(Capteur_TH.getHumidity(), *BTModule);
		myPrintln(" %\n", *BTModule);
	}
	else myPrintln("Humidite : N/C\n", *BTModule);
}

void CoolStick::getLuminosity()
{
//Mesure:
	Capteur_L.readData();
//Ecriture:
	myPrint("\n\tLuminosite : ", *BTModule);
	myPrint(int(map(Capteur_L.getVal(), 0, 1023, 0, 100)), *BTModule);
	myPrint(" %\n", *BTModule);
	return;
}

void CoolStick::getLuminosityR()
{
//Mesure:
	Capteur_L.readRVal();
//Ecriture:
	myPrint("Luminosite :\n\tResistance : ", *BTModule);
	myPrint(Capteur_L.getRVal(), *BTModule);
	myPrint(" Ohms\n", *BTModule);
	return;
}

void CoolStick::getLuminosityB()
{
//Mesure:
	Capteur_L.readBrightness();
//Ecriture:
	myPrint("Luminosite :\n\tBrillance : ", *BTModule);
	myPrint(Capteur_L.getBVal(), *BTModule);
	myPrint(" %\n", *BTModule);
	return;
}

void CoolStick::getMoisture()
{
//Mesure:
	Capteur_M.readVal();
//Ecriture:
	myPrint("Hydrometrie : ", *BTModule);
	myPrint(Capteur_M.getVal(), *BTModule);
	myPrint(" %\n", *BTModule);
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
////////////////////////////////FONCTION STICK/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CoolStick::checkMessage()
{
	ReceivedBuffer = "";
	ReceivedBuffer = myRead(*BTModule);
}

void CoolStick::treatMessage()
{
	
	if((ReceivedBuffer.equals("?")||ReceivedBuffer.equals("help"))) MessageLevel = 10;
	else if(ReceivedBuffer.equals("Temp")) MessageLevel = 1;
	else if(ReceivedBuffer.equals("Hum")) MessageLevel = 2;
	else if(ReceivedBuffer.equals("Lum")) MessageLevel = 3;
	else if(ReceivedBuffer.equals("RLum")) MessageLevel = 31;
	else if(ReceivedBuffer.equals("BLum")) MessageLevel = 32;
	else if(ReceivedBuffer.equals("Hydro")) MessageLevel = 4;
	else if(ReceivedBuffer.equals("All")) MessageLevel = 5;
	else MessageLevel = 0;
}

void CoolStick::takeMeasure()
{
	SendingBuffer = "";
//Hydrometrie:
	Capteur_M.readVal();
//Luminosite:
	Capteur_L.readData();
	Capteur_L.readRVal();
	Capteur_L.readBrightness();
//Temperature:	
	if(!IsDHTHere) Capteur_T.readData();
	else
	{
		DHTTimer[1] = millis();
		if(DHTTimer[1] - DHTTimer[0] > DHTMaxFreq)
		{
			DHTTimer[0] = DHTTimer[1];
			Capteur_TH.readSensor();
		}
	}
}

void CoolStick::mountMessage()
{
	switch(MessageLevel)
	{
		case 10:			//Infos
			SendingBuffer.concat("\nCoolStick peut vous fournir les données suivantes :\n\t->Hydratation du sol (Hydro)\n\t->Luminosite(Lum)\n\t->Temperature (Temp)");
			if(IsDHTHere) SendingBuffer.concat("\n\t->Humidite Relative (Hum)\n\n");
			break;
		case 5:			//General
			SendingBuffer.concat("Donnees Globales :\n");
			SendingBuffer.concat("\tHydratation : ");
			SendingBuffer.concat(Capteur_M.getVal());
			SendingBuffer.concat(" %\n");
			SendingBuffer.concat("\tLuminosite :");
			SendingBuffer.concat(Capteur_L.getBVal());
			SendingBuffer.concat(" %\n");
			if(IsDHTHere)
			{
				if(Capteur_TH.getTemperature() != -1)
				{
					SendingBuffer.concat("\tTemperature :");
					SendingBuffer.concat(Capteur_TH.getTemperature());
					SendingBuffer.concat(" °C\n");
					SendingBuffer.concat("\tHumidite :");
					SendingBuffer.concat(Capteur_TH.getHumidity());
					SendingBuffer.concat(" %\n");
				}
				else SendingBuffer.concat("\tCapteur DHT en Erreur\n");	
			}
			else
			{
				SendingBuffer.concat("\tTemperature :");
				SendingBuffer.concat(Capteur_T.getTemperature());
				SendingBuffer.concat(" °C\n");
			}
			break;
		case 1:			//Temperature
			if(IsDHTHere)
			{
				SendingBuffer.concat("Temperature :");
				SendingBuffer.concat(Capteur_TH.getTemperature());
				SendingBuffer.concat(" °C\n");
			}
			else
			{
				SendingBuffer.concat("Temperature :");
				SendingBuffer.concat(Capteur_T.getTemperature());
				SendingBuffer.concat(" °C\n");
			}
			break;
		case 2:			//Humidite
			if(IsDHTHere)
			{
				SendingBuffer.concat("Humidite :");
				SendingBuffer.concat(Capteur_TH.getHumidity());
				SendingBuffer.concat(" %\n");
			}
			else SendingBuffer.concat("Humidite : N/C\n");
			
			break;
		case 3:			//Luminosite
			SendingBuffer.concat("Luminosite :");
			SendingBuffer.concat(Capteur_L.getBVal());
			SendingBuffer.concat(" %\n");
			break;
		case 31:		//Resistance Luminosite
			SendingBuffer.concat("Resistance Luminosite :");
			SendingBuffer.concat(Capteur_L.getRVal());
			SendingBuffer.concat(" ohms\n");
			break;
		case 32:		//Brillance Luminosite
			SendingBuffer.concat("Brillance Luminosite :");
			SendingBuffer.concat(Capteur_L.getVal());
			SendingBuffer.concat('\n');
			break;
		case 4:			//Hydrometrie
			SendingBuffer.concat("Hydratation : ");
			SendingBuffer.concat(Capteur_M.getVal());
			SendingBuffer.concat(" %\n");
			break;
		default:		//Erreurs
			break;
	}
}

void CoolStick::sendMessage()
{
	if(MessageLevel != 0) myPrintln(ReceivedBuffer, *BTModule);
	else if(!ReceivedBuffer.equals("")) myPrintln(F("Demande non prise en compte...\n\t----------\n\nQuel parametre voulez vous connaitre?"), *BTModule);
	myPrint(SendingBuffer, *BTModule);
	if(!SendingBuffer.equals("")) myPrintln(F("\t----------\n\nQuel parametre voulez vous connaitre?"), *BTModule);
	SendingBuffer = "";
	myFlush(*BTModule);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////