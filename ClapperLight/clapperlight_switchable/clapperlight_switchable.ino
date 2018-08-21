//Project: Sound Sensor
//Function: You can see the value of sound intensity on Serial Monitor.
//When the volume reaches to a certain value, the LED will light up. 
//**********************************************************************
const int ledPin = 12;    //LED attached to digital pin 12
const int soundPin = A0;  //Sound sensor attached to analog pin A0
int val = 0;              //the value of sound sensor
//*********************************************************************
void setup()
{
  pinMode(soundPin,INPUT);         //set soundPin as INPUT
  pinMode(ledPin,OUTPUT);          //set ledPin as OUTPUT
  Serial.begin(300);              //initialize serial communications at 9600 bps
}

void loop()
{
  val = analogRead(soundPin);      //read the value of sound sensor
  Serial.println(val);             //print the value of sound sensor on serial monitor
  if(val > 26)                    //if the value is greater than 27
  {
    digitalWrite(ledPin,HIGH);     //turn the LED on
    val = analogRead(soundPin);    // Listen for another sound
    if(val > 26) {
      digitalWrite(ledPin,LOW);
    }
  }
}
