/*
 * Austin Tarango
 * Jan 10, 2017
 * Sunrise Alarm Code
 */
#include <DS3231.h> //Include the clock library
#include "RTClib.h"
#include <Wire.h>
RTC_DS1307 RTC;

// Changable Vars
int fadeTime = 30; // How long the light will fade to max
int setHour = 15; // Set hours to wake (military time)
int setMin = 55; // Set minute to wake
int uled = 9; // Set pinout with with PWM

// Set up Vars
DS3231  rtc(SDA, SCL);
Time t;
void start();
 
void setup()
{
  pinMode(uled, OUTPUT);
  Serial.begin(9600); // Match to serial monitor

    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop()
{
  DateTime now = RTC.now(); 
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();    
    delay(1000);
  
  if (t.hour == setHour && t.min == setMin) // Check if it's time to wake up!
  {
    start();
  }
  
  // Wait one second before repeating
  delay (1000);
}

void start()
{
  DateTime now = RTC.now();  // trying to get time to read during start func
  
  // Fix for SB LED
  analogWrite(uled, 1); // normal code
    Serial.print(now.minute(), DEC); // start inserted code
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); // end inserted code
    
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 2);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 3);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 4);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 4);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 5);
  delay((fadeTime * 60000)/50);


  // Fade script
  for (int i = 6 ; i <= 255; i++)
  {
    analogWrite(uled, i);
    delay(((fadeTime * 60000)/306));
    Serial.print(" mil sec ");
    Serial.print(((fadeTime * 60000)/306));
    Serial.print(" PWM " );
    Serial.print(i);


  }
  
  delay(20000); // Stay Bright
  analogWrite(uled, 0); // Turn off
  
}
