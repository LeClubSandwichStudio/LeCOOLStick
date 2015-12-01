#include <CoolStick.h>
#include <SoftwareSerial.h>
#define RxD 0
#define TxD	1

SoftwareSerial	BTModule(RxD,TxD);
CoolStick	MyStick;
int	MoistPin = A5;
int LumPin = A4;
int TempPin = 7;

void setup()
{
	Serial.begin(9600);
	MyStick.attachBlueTooth(&BTModule);
	MyStick.begin(RxD, TxD, MoistPin, LumPin, TempPin);
}

void loop()
{
	MyStick.checkMessage();
	MyStick.treatMessage();
	MyStick.takeMeasure();
	MyStick.mountMessage();
	MyStick.sendMessage();
}
