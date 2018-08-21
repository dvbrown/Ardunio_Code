/*
 Here the led and the rtc are working correctly
 The hardware interupt breaks the LED so its flashing
 */


#include <DS3231.h> //Include the clock library
#include <TimerOne.h>
#include "TM1637.h"
#include <Wire.h>
#include "RTClib.h"

#define ON 1
#define OFF 0

// Changable Vars
int fadeTime = 30; // How long in minutes the light will fade to max
int setHour = 16; // Set hours to wake (military time)
int setMin = 11; // Set minute to wake
int uled = 9; // Set pinout with with PWM

// Setup clock
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second;
unsigned char minute = 12;
unsigned char hour = 12;

// Set up RTC Vars
RTC_DS3231 rtc;
Time t;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define CLK 3//pins definitions for TM1637 and can be changed to other ports    
#define DIO 2
TM1637 tm1637(CLK,DIO);

void setup()
{
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  pinMode(uled, OUTPUT);
  Serial.begin(9600); // Match to serial monitor
  delay(2000); // wait for console opening

   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
   }
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // Daniel thinks next bit goes in loop  
  tm1637.set();
  tm1637.init();

  // These lines below affect the flashing time on the LED
  //Timer1.initialize(500000);//timing for 500ms
  //Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR  
}

void loop()
{
  DateTime now = rtc.now();
    // Print time to the serial monitor
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

  // Display the time on the LCD
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }

  hour = now.hour();
  minute = now.minute();
  
  //if (t.hour == setHour && t.min == setMin)
  if (hour == setHour && minute == setMin)
  {
    Serial.print("The LED should LIGHT UP!!!");
    start();
  }
  
  // Wait one second before repeating
  delay (1000);
}

void start()
{
  // Fix for SB LED
  analogWrite(uled, 1);
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

void TimingISR()
{
  halfsecond ++;
  Update = ON;
  if(halfsecond == 2){
    second ++;
    if(second == 60)
    {
      minute ++;
      if(minute == 60)
      {
        hour ++;
        if(hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;  
  }
 // Serial.println(second);
  ClockPoint = (~ClockPoint) & 0x01;
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
  Update = OFF;
}
