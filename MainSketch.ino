/*
 * Aaron Meadows
 * Start: 6/30/19
 * To control a mars rover for VBS 2019 at Liberty Christian Church.
 * Finish: TBA
 */

//Librarys
#include <Wire.h>
#include <Adafruit_VL53L0X.h>


//----Variables----//


//-Movement Control-//
const int motorLPin1 = 6;
const int motorLPin2 = 9;
const int motorRPin1 = 10;
const int motorRPin2 = 11;

int accelRate = 15;
int turnStart;
int turnTime;

//--Light Control--//
const int redPin = 3;
const int grePin = 4;
const int bluPin = 5;

//-Distance Sensor-//
//Laser sensor variables
Adafruit_VL53L0X sensor;

//Distance varibles
long duration;
long mm;
int tooClose = 300; // In mm

void setup() {
  Serial.begin(9600);
  
  sensorLaserSetup();

  //Motor Pins
  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);
  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);

  //Light Pins
  pinMode(2, OUTPUT); //BLUE
  pinMode(3, OUTPUT); //RED
  pinMode(4, OUTPUT); //GREEN
}

void loop() {
  if (!isObjectInPath())
  {
    digitalWrite(4, HIGH); //Green Light
    digitalWrite(3, LOW); //Red Light Off
    forward();
  }
  else
  {
    digitalWrite(4, LOW); //Green Light off
    digitalWrite(3, HIGH); //Red Light
    freeze();
    delay(500);
    while(isObjectInPath())
    {
      turnR();
      turnStart = millis();
      if (millis() - turnStart > 10000)
      {
        freeze();
        exit(0);
      }
    }
    freeze();
  }
}

void sensorLaserSetup()
{
  //Setup Sensor
  pinMode(12,INPUT_PULLUP);
  digitalWrite(12,HIGH);
  
  sensor.begin();
}

void pingL()
{
  VL53L0X_RangingMeasurementData_t measure;
  
  sensor.rangingTest(&measure, false); //true will add debug data to printout
  
  if (measure.RangeStatus != 4) 
  {  
    // phase failures have incorrect data
    Serial.print("mm L "); 
    
    mm = measure.RangeMilliMeter - 10; //It seemed to be precise but not accurate so linear adjustments were made
    
    Serial.println(mm);
  } 
  else 
  {
    mm = 9999;
    Serial.println("Error");
  }
}

bool isObjectInPath()
{
  long sum = 0;
  
  //Gather 5 distances
  for(int i = 0; i < 5; i++)
  {
    pingL(); //Update distance
    sum = mm + sum;
  }
  
  //Average the distances
  mm = sum / 5;
  Serial.print("Averaged: ");
  Serial.println(mm);
  
  if (mm < tooClose)
    return true;      
  else
    return false;
}

//----------------------FORWARD-----------------//
void forward()
{
    analogWrite(motorLPin1, 255);
    analogWrite(motorRPin1, 255);
    analogWrite(motorLPin2, 0);
    analogWrite(motorRPin2, 0);
}
//------------------ACCELERATE------------------//
void accelerate()
{
  for (int i=0; i<=255; i++)
  {
    analogWrite(motorLPin1, i);
    analogWrite(motorRPin1, i);
    delay(accelRate);
  }
}
//----------------REVERSE-----------------------//
void reverse()
{
    analogWrite(motorLPin1, 0);
    analogWrite(motorRPin1, 0);
    analogWrite(motorLPin2, 255);
    analogWrite(motorRPin2, 255);
}
//----------------DECCELERATE-------------------//
void deccelerate()
{
  for (int i=255; i>=0; i--)
  {
    analogWrite(motorLPin2, i);
    analogWrite(motorRPin2, i);
    delay(accelRate);
  }
}
//--------------------TURNR---------------------//
void turnR()
{
    analogWrite(motorRPin1, 150);
    analogWrite(motorLPin2, 150);
    analogWrite(motorRPin2, 0);
    analogWrite(motorLPin1, 0);  
}
//---------------------STOP---------------------//
void freeze()
{
    analogWrite(motorLPin1, 0);
    analogWrite(motorRPin1, 0);
    analogWrite(motorLPin2, 0);
    analogWrite(motorRPin2, 0);
}

//--------------------LIGHTS--------------------//
void setLight(String color, int brightness)
{/*
  switch(color)
  {
    case "R":

    break;
   
  }*/
}
