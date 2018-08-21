//Project: Sound Sensor
//Function: You can see the value of sound intensity on Serial Monitor.
//When the volume reaches to a certain value, the LED will light up. 
//**********************************************************************
const int ledPin = 12;    //LED attached to digital pin 12
const int soundPin = 7;   //Sound sensor attached to digital pin 7
int val = 0;              //the value of sound sensor
//*********************************************************************
void setup()
{
  pinMode(soundPin,INPUT);         //set soundPin as INPUT
  pinMode(ledPin,OUTPUT);          //set ledPin as OUTPUT
  Serial.begin(9600);              //initialize serial communications at 9600 bps
}

void loop()
{
  val = digitalRead(soundPin);      //read the value of sound sensor
  Serial.println(val);             //print the value of sound sensor on serial monitor
  if(val == HIGH)                   //if Sensor active than
  {
    digitalWrite(ledPin,HIGH);     //turn the LED on
  }
  else
  {
    digitalWrite(ledPin,LOW);      //turn the LED off
  }
}
