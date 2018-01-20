//including libraries
#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
//#include <Nokia5110.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//setting up ds18b20
#define ONE_WIRE_BUS 15
#define TEMPERATURE_PRECISION 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//constants
const char* ssid     = "Henry";
const char* password = "5451F08EF688554497EF34306A";

//variables
DeviceAddress tempDeviceAddress[6];
DeviceAddress tempDeviceAddressa[6]={
	{ 0x28, 0xFF, 0x9F, 0x51, 0xA1, 0x15, 0x01, 0xD7 }, 
	{ 0x28, 0xFF, 0xE8, 0x78, 0xA1, 0x15, 0x04, 0x37 },
	{ 0x28, 0xFF, 0x44, 0x15, 0xA1, 0x15, 0x01, 0x17 },
	{ 0x28, 0xFF, 0x46, 0x7C, 0xA1, 0x15, 0x01, 0x97 },
	{ 0x28, 0xFF, 0x70, 0x7F, 0x65, 0x15, 0x02, 0x28 },
	{ 0x28, 0xFF, 0x9A, 0x4C, 0xA1, 0x15, 0x01, 0xA0 }
};
DeviceAddress tempDeviceAddressp;
int numberOfDevices;
float Temperatures[6];
int CAP;
String CAPt;
float Tpr;
String Tprt;
char tisk[5];
String Temperaturest[6];

//objects
WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27,16,2);//3F

//functions
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void sset();
//void wset();
void dset();
void show();
void mer();


void setup(){
    Serial.begin(115200);
	sset();
	wset();
	dset();
	
}

void loop(){
	mer();
	show();
}

void dset(){
	lcd.begin(21, 22);                      // initialize the lcd 
  // Print a message to the LCD.
  	lcd.noBacklight();
	  Serial.println(lcd.getBacklight());
	  delay(1000);
	  lcd.backlight();
	  Serial.println(lcd.getBacklight());
}

void mer(){
	Tpr = 0;
	for(int i; i<6; i++){
		Temperatures[i] = sensors.getTempC(tempDeviceAddressa[i]);
		Tpr = Tpr + sensors.getTempC(tempDeviceAddressa[i]);
		Temperaturest[i] = String(sensors.getTempC(tempDeviceAddressa[i]));
		Serial.println("");
		Serial.println(Temperaturest[i]);
		Serial.print("Soucet:");
		Serial.println(Tpr/6);
	}
	Serial.println("");

	Tpr = Tpr/6;
	CAP = (Tpr-20)/0.65;
	Tprt = String(Tpr, DEC);
  	CAPt = String(CAP, DEC); 
}

void show(){
	lcd.setCursor(0,0);
  	lcd.print("Tpr:");
  	lcd.setCursor(0,1);
  	lcd.print("CAP:");

	lcd.setCursor(4,0);
  	lcd.print(Tprt.c_str());
  	lcd.setCursor(4,1);
  	lcd.print(CAPt.c_str());
	delay(2000);
	lcd.clear();
	lcd.setCursor(0,0);
  	lcd.print("1:");
  	lcd.setCursor(0,1);
  	lcd.print("2:");

	lcd.setCursor(2,0);
  	lcd.print(Temperaturest[0].c_str());
  	lcd.setCursor(2,1);
  	lcd.print(Temperaturest[1].c_str());
	delay(2000);
	lcd.clear();
	lcd.setCursor(0,0);
  	lcd.print("3:");
  	lcd.setCursor(0,1);
  	lcd.print("4:");

	lcd.setCursor(2,0);
  	lcd.print(Temperaturest[2].c_str());
  	lcd.setCursor(2,1);
  	lcd.print(Temperaturest[3].c_str());
	delay(2000);
	lcd.clear();
	lcd.setCursor(0,0);
  	lcd.print("5:");
  	lcd.setCursor(0,1);
  	lcd.print("6:");

	lcd.setCursor(2,0);
  	lcd.print(Temperaturest[4].c_str());
  	lcd.setCursor(2,1);
  	lcd.print(Temperaturest[5].c_str());
	delay(2000);
	
}

void wset(){
	Serial.println("Setting up WiFi:");
	WiFi.begin(ssid, password);
  	delay(500);
	Serial.println(WiFi.status());
	for (int i; i < 20; i++){
   		if(WiFi.status() != WL_CONNECTED) {
			Serial.print(".");
			delay(500);
  		}
	}
	if(WiFi.status() != WL_CONNECTED) {
		Serial.println("An error has occured, trying again.");
		wset();
	}else{
		Serial.println("Connected.");
		Serial.println("IP address: ");
  		Serial.println(WiFi.localIP());
   		server.begin();
	}
	delay(500);
}

void sset(){
	Serial.println("Setting up ds18b20:");
    sensors.begin();
    Serial.println("Starting sensors...");
    Serial.print("Found ");
	numberOfDevices = sensors.getDeviceCount();
    Serial.print(numberOfDevices, DEC);
    Serial.println(" ds18b20");
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) Serial.println("ON");
    	else Serial.println("OFF");
    for(int i=0;i<numberOfDevices; i++){
    	// Search the wire for address
		if(sensors.getAddress(tempDeviceAddress[i], i))
		{
			Serial.print("Found device ");
			Serial.print(i, DEC);
			Serial.print(" with address: ");
			printAddress(tempDeviceAddress[i]);
			Serial.println();
			
			Serial.print("Setting resolution to ");
			Serial.println(TEMPERATURE_PRECISION, DEC);
			
			// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
			sensors.setResolution(tempDeviceAddress[i], TEMPERATURE_PRECISION);
			
			Serial.print("Resolution actually set to: ");
			Serial.print(sensors.getResolution(tempDeviceAddress[i]), DEC); 
			Serial.println();
		}else{
			Serial.print("Found ghost device at ");
			Serial.print(i, DEC);
			Serial.print(" but could not detect address. Check power and cabling");
		}
    }
	int pom;

}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}