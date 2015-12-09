#include <CoolStick.h>
#include <SoftwareSerial.h>
#define RxD 8
#define TxD	9

SoftwareSerial	BTModule(RxD,TxD);
CoolStick	MyStick;
int	MoistPin = A5;
int LumPin = A4;
int TempPin = 2;

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
