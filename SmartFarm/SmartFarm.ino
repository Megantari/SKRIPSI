#include "Akses_jaringan.h";
#include "Parameter.h";
/* ESP & Blynk */
#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WidgetLED PUMPs(V0);  // Echo signal to Sensors Tab at Blynk App
WidgetLED PUMPa(V5); // Echo signal to Actuators Tab at Blynk App
WidgetLED LAMPs(V1);  // Echo signal to Sensors Tab at Blynk App
WidgetLED LAMPa(V6); // Echo signal to Actuators Tab at Blynk App

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;


/* Lcd */
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

/* DHT22*/
#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);

/* DS18B20 Temperature Sensor */
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void setup()
{

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(PUMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(LAMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(SENSORS_READ_BUTTON, INPUT_PULLUP);
  pinMode(soilMoisterVcc, OUTPUT);
  
  //Blynk.config(auth, server, port);
  //Blynk.connect();
Blynk.begin(auth, ssid, pass ,server ,port);


 lcd.init();
 lcd.backlight();
 startlcd();
 delay(5000);
 lcd.init();
 lcd.backlight();
 varlcd();
  dht.begin();
  DS18B20.begin();

  PUMPs.off();
  LAMPs.off();
  PUMPa.off();
  LAMPa.off();
  digitalWrite(PUMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
  digitalWrite(LAMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
  digitalWrite (soilMoisterVcc, LOW); 
  waitButtonPress (SHOW_SET_UP); // Wait for Sensor Button to be pressed 
  lcd.clear();
  startTimers();
}

void loop()
{
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}

/****************************************************************
* Read remote commands 
****************************************************************/
BLYNK_WRITE(3) // Pump remote control
{
  int i=param.asInt();
  if (i==1) 
  {
    pumpStatus = !pumpStatus;
    aplyCmd();
  }
}

BLYNK_WRITE(4) // Lamp remote control
{
  int i=param.asInt();
  if (i==1) 
  {
    lampStatus = !lampStatus;
    aplyCmd();
  }
}

/****************************************************************
* Read local commands (Pump and Lamp buttons are normally "HIGH"):
****************************************************************/
void readLocalCmd() 
{  
  boolean digiValue = debounce(PUMP_ON_BUTTON);
  if (!digiValue) 
  {
    pumpStatus = !pumpStatus;
    aplyCmd();
  }

  digiValue = debounce(LAMP_ON_BUTTON);
  if (!digiValue) 
  {
    lampStatus = !lampStatus;
    aplyCmd();
  }

  digiValue = debounce(SENSORS_READ_BUTTON);
  if (!digiValue) 
  {
    turnOffLcd = !turnOffLcd;
    if (!turnOffLcd)
    {
      lcd.init();
      lcd.backlight();
      lcd.setCursor(2,0); lcd.print("UPDATING SENSORS");
      
      getDhtData();
      getSoilMoisterData();
      getSoilTempData();
      //lcd.init();
      lcd.clear();
      displayData();
    }else
    //lcd.noBacklight(); 
    lcd.clear();
  }
}

/***************************************************
* Receive Commands and act on RELAYs - Logic LOW
****************************************************/
void aplyCmd()
{
  if (pumpStatus == 1) 
  {
    Blynk.notify("SmartFarm: Warning ==>> Pump ON"); 
    digitalWrite(PUMP_PIN, HIGH); // To be used with Relay module (inverted logic: activate with LOW)
    if (!turnOffLcd) displayData();
    PUMPs.on();
    PUMPa.on();
  }
  else
      {
        digitalWrite(PUMP_PIN,LOW ); // To be used with Relay module (inverted logic: normally HIGH)
        if (!turnOffLcd) displayData();
        PUMPs.off();
        PUMPa.off();
      }
  
  if (lampStatus == 1) 
  {
    Blynk.notify("SmartFarm: Warning ==>> Lamp ON");
    digitalWrite(LAMP_PIN, HIGH); // To be used with Relay module (inverted logic: activate with LOW)
    if (!turnOffLcd) displayData();
    LAMPs.on();
    LAMPa.on();
  }
  else
      {
        digitalWrite(LAMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
        if (!turnOffLcd) displayData();
        LAMPs.off();
        LAMPa.off();
      }
}

/***************************************************
* Automatically Control the Plantation based on sensors reading
****************************************************/
void autoControlPlantation(void)
{ 
  if (soilMoister < DRY_SOIL) 
  {
    Pompa_Nyala();
  }

  if (airTemp < COLD_TEMP) 
  {
    Lampu_Nyala();
  }
}

/***************************************************
* Pompa nyala + waktu
****************************************************/
void Pompa_Nyala()
{
  pumpStatus = 1;
  aplyCmd();
  delay (TIME_PUMP_ON*1000);
  pumpStatus = 0;
  aplyCmd();
}

/***************************************************
* Lampu nyala + waktu
****************************************************/
void Lampu_Nyala()
{
  lampStatus = 1;
  aplyCmd();
  delay (TIME_LAMP_ON*1000);
  lampStatus = 0;
  aplyCmd();
}

/***************************************************
 * Kirim data ke Blynk
 **************************************************/
void sendUptime()
{
  Blynk.virtualWrite(10, airTemp); //virtual pin V10
  Blynk.virtualWrite(11, airHum); // virtual pin V11
  Blynk.virtualWrite(12, soilMoister); // virtual pin V12
  Blynk.virtualWrite(13, soilTemp); //virtual pin V13
}
