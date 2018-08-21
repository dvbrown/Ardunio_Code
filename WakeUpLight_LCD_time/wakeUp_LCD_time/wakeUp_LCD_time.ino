#include <DS3231.h> //Include the clock library
#include <TimerOne.h>
#include "TM1637.h"

#define ON 1
#define OFF 0

// Changable Vars
int fadeTime = 30; // How long in minutes the light will fade to max
int setHour = 15; // Set hours to wake (military time)
int setMin = 25; // Set minute to wake
int uled = 9; // Set pinout with with PWM

// Setup clock
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second;
unsigned char minute = 12;
unsigned char hour = 12;

// Set up Vars
DS3231  rtc(SDA, SCL);
Time t;

#define CLK 3//pins definitions for TM1637 and can be changed to other ports    
#define DIO 2
TM1637 tm1637(CLK,DIO);

void start();
 
void setup()
{
  pinMode(uled, OUTPUT);
  Serial.begin(9600); // Match to serial monitor
  rtc.begin();

  t = rtc.getTime();
  hour = t.hour;
  minute = t.min;  
  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR  
}

void loop()
{
  //t = rtc.getTime(); // Make a time class called 't'

  // Display the time on the LCD
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }

  // Wakeup clock
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

  if (t.hour == setHour && t.min == setMin) // Check if it's time to wake up!
  {
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
