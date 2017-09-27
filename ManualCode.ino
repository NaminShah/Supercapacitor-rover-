// This #include statement was automatically added by the Particle IDE.
#include "lib1.h"

// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

//#define BLYNK_PRINT Serial

#ifndef TOKEN
#define TOKEN "A1E-PtCy3gqTg9ZNHwCf2HqiGn2eTPQQFb"  // Put here your Ubidots TOKEN
#endif

Ubidots ubidots(TOKEN);

//#include <blynk.h>
const int enA = D0;//rightmotor
const int in1 = D4;
const int in2= D5;

const int enB = D1;//leftmotor
const int in3 = D6;
const int in4 = D7;
 int rawx;
 int rawy;
 
 int fb;
 int rl;
 
 int swag = 0;
 
  float vPow = 3.5;
 float r1 = 100000;
 float r2 = 10000;
 
 
 
static  float startBval; // initial magnetic field as determined by hall effect sensor, varies from boot on to boot on
static int current;
 
    unsigned long ch1; // Here's where we'll keep our channel values
    unsigned long ch2;
    unsigned long ch1AVG;
    unsigned long ch2AVG;
    
    unsigned long tiempo;
    
// Rover control with blink app

/*You should get Auth Token in the Blynk App.
//Go to the Project Settings (nut icon).
char auth[] = "7fca16430dfc4756a110df4b31aec5d4";
BLYNK_WRITE(V1) {
    rawx = param[0].asInt();
    rawy = param[1].asInt();
    pinMode(enA,OUTPUT);
    pinMode(inFa,OUTPUT);
    pinMode(inFb,OUTPUT);
    pinMode(inBa,OUTPUT);
    pinMode(inBb,OUTPUT);
    pinMode(enB,OUTPUT);
     
  int xnew = abs(128 - rawx)+1;
 int ynew = abs(128 - rawy) -1;
 if(rawy  > 128)//forward
    {
        analogWrite(enB, 2*ynew);
        analogWrite(enA, 2*ynew);
        digitalWrite(inFb,HIGH);
        digitalWrite(inFa,HIGH);
        digitalWrite(inBb,LOW);
        digitalWrite(inBa,LOW);
        digitalWrite(D0,HIGH);
        currentDraw();
       }
  else if ( rawy < 128)//backward  
    {
        analogWrite(enB, 2*ynew);
        analogWrite(enA, 2*ynew);
        digitalWrite(inFb,LOW);
        digitalWrite(inFa,LOW);
        digitalWrite(inBb,HIGH);
        digitalWrite(inBa,HIGH);
        digitalWrite(D0,HIGH);
        
    currentDraw();
      }
  else 
    {
        analogWrite(enB,0);
        analogWrite(enA,0);
    }
        
  // Do something with x and y
  Serial.print("X = ");
  Serial.print(rawx);
  Serial.print("; Y = ");
  Serial.println(rawy);
}
BLYNK_WRITE(V2)
{
    pinMode(enA,OUTPUT);
    int leftState = param.asInt();//LEFT button state
    if(leftState==1)
    analogWrite(enA,0);
 }
BLYNK_WRITE(V3)
{ 
    pinMode(enB,OUTPUT);
    
    int rightState =param.asInt();//RIGHT button state
     
     if(rightState == 1)
     analogWrite(enB,0);
 }
BLYNK_WRITE(V4)
{
    int biii = param.asInt();
    if (biii == 1)
    currentDraw();
    
}*/
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

void currentDraw()
{
    
    
     float c1 = analogRead(A3) - startBval;
  //float v = (analogRead(A4) * vPow) / 4095.0;
   //float v2 = v / (r2 / (r1 + r2));
   
   float I = (c1*3.14)/1000;
   float FPR =(analogRead(A2));
   
   float BPR = analogRead(A1);
    //float value3 = analogRead(A2);
    //ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("frontPR", FPR);
    ubidots.add("backPR", BPR);
    
    
    ubidots.add("Current", I);
   //ubidots.add("Voltage", v2);
    
    //ubidots.add("Variable_Name_Three", value3);
    ubidots.setMethod(TYPE_TCP); 
    //Set to TCP the way to send data
   ubidots.sendAll();
     
}

void voltage()
{
      float v = (analogRead(A4) * vPow) / 4095.0;
   float v2 = v / (r2 / (r1 + r2));
    
   
   
    //float value3 = analogRead(A2);
    //ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    //ubidots.add("Current", v1);
   ubidots.add("Voltage", v2);
   
   if ( v2 < 3.5)
   {
       digitalWrite(A0,HIGH); // 
   }
    
    //ubidots.add("Variable_Name_Three", value3);
    ubidots.setMethod(TYPE_TCP); 
    //Set to TCP the way to send data
   ubidots.sendAll();
     
}

/*BLYNK_READ(V5)
    {      
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
 Blynk.virtualWrite(V5,(analogRead(A1)-startBval));
    }
*/

void setup()
{
    
        Serial.begin(9600);
  // Debug console
  
  for(int x =0; x<= 9; x++)
    {
      ch1AVG += pulseIn(D2,(uint8_t)HIGH);
      ch2AVG += pulseIn(D3,(uint8_t)HIGH);
    }
  
  ch1AVG= ch1AVG/10;
  ch2AVG = ch2AVG/10;
  
  pinMode(enA,OUTPUT);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(in3,OUTPUT);
    pinMode(in4,OUTPUT);
    pinMode(enB,OUTPUT);

  //delay(5000); // Allow board to settle
  //Blynk.begin(auth);
   
   
   
  //startBval= analogRead(A1);
 }
 
 
 
 unsigned long pulseIn(uint16_t pin, uint8_t state) {
    
    GPIO_TypeDef* portMask = (PIN_MAP[pin].gpio_peripheral); // Cache the target's peripheral mask to speed up the loops.
    uint16_t pinMask = (PIN_MAP[pin].gpio_pin); // Cache the target's GPIO pin mask to speed up the loops.
    unsigned long pulseCount = 0; // Initialize the pulseCount variable now to save time.
    unsigned long loopCount = 0; // Initialize the loopCount variable now to save time.
    unsigned long loopMax = 20000000; // Roughly just under 10 seconds timeout to maintain the Spark Cloud connection.
    
    // Wait for the pin to enter target state while keeping track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) != state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
    }
    
    // Iterate the pulseCount variable each time through the loop to measure the pulse length; we also still keep track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) == state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
        pulseCount++;
    }
    
    // Return the pulse time in microseconds by multiplying the pulseCount variable with the time it takes to run once through the loop.
    return pulseCount * 0.405; // Calculated the pulseCount++ loop to be about 0.405uS in length.
}



void loop()
{
// Blynk.run();

// float value1 = analogRead(A0);
ch1 = pulseIn(D3,(uint8_t )HIGH);
ch2 = pulseIn(D2,(uint8_t )HIGH);

     fb= (ch1 - ch1AVG);
    
     rl = (ch2 - ch2AVG);
    
    if ( fb > 255)
    fb = 255;
    else if(fb < -255)
    fb = -255;
    else if ( fb < 75 & fb > -75)
    fb = 0;
    
    if ( rl < 300& rl > -300)
    rl = 0;
    
  
 
 
  tiempo = millis();
    if(digitalRead(D0) == HIGH)
    {
        startBval = analogRead(A3);
    }
    
    
    if( rl > 0 &  rl < 750) //right
{
     if((fb)<0)//backward
      {
      analogWrite(enB,(abs(fb)));
      analogWrite(enA,0);
      digitalWrite(in1,HIGH);
      digitalWrite(in3,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in4,LOW);
      }
      else if ((fb)>0)//forward
       {

        analogWrite(enB,(abs(fb)));
        analogWrite(enA,0);
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,HIGH);

      }
      else 
      {
        analogWrite(enB,0);
        analogWrite(enA,0);
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in4,LOW);
  
      }

 
}
else if( rl > 750)
{
    if(fb < 0)//backward
     {
      analogWrite(enB,(abs(fb)));
      analogWrite(enA,(abs(fb)));
      digitalWrite(in1,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in2,HIGH);
       digitalWrite(in4,LOW);
    }
    else if ((fb) > 0)//forward
      {

 
      analogWrite(enB,((abs(fb))));
      analogWrite(enA,((abs(fb))));
      digitalWrite(in1,HIGH);
      digitalWrite(in3,LOW);
       digitalWrite(in2,LOW);
     digitalWrite(in4,HIGH);

      }
     else 
      {
      analogWrite(enB,0);
      analogWrite(enA,0);
      digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in4,LOW);
  
      }

} 
else if( rl < 0 & rl > -600)
{
    if((fb) < 0)//backward
     {
      analogWrite(enB,0);
      analogWrite(enA,((abs(fb))));
      digitalWrite(in1,HIGH);
      digitalWrite(in3,HIGH);
      digitalWrite(in2,LOW);
       digitalWrite(in4,LOW);
    }
    else if ((fb) >0)//forward
      {

 
      analogWrite(enB,0);
      analogWrite(enA,((abs(fb))));
      digitalWrite(in1,LOW);
      digitalWrite(in3,LOW);
       digitalWrite(in2,HIGH);
     digitalWrite(in4,HIGH);

      }
     else 
      {
      analogWrite(enB,0);
      analogWrite(enA,0);
      digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in4,LOW);
  
      }

} 

else if ( rl < -600)
{
    if((fb)<0)//backward
      {
      analogWrite(enB,((abs(fb))));
      analogWrite(enA,((abs(fb))));
      digitalWrite(in1,HIGH);
      digitalWrite(in3,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in4,HIGH);
      }
      else if ((fb)>0)//forward
       {

 
        analogWrite(enB,((abs(fb))));
        analogWrite(enA,((abs(fb))));
        digitalWrite(in1,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,LOW);

      }
      else 
      {
        analogWrite(enB,0);
        analogWrite(enA,0);
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in4,LOW);
  
      }
}
else 
{
  
  
    if((fb) < 0)//backward
      {
      analogWrite(enB,((abs(fb))));
      analogWrite(enA,((abs(fb))));
      digitalWrite(in1,HIGH);
      digitalWrite(in3,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in4,LOW);
      }
      else if ((fb)>0)//forward
       {

 
        analogWrite(enB,((abs(fb))));
        analogWrite(enA,((abs(fb))));
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,HIGH);

      }
      else 
      {
        analogWrite(enB,0);
        analogWrite(enA,0);
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in4,LOW);
        
        startBval = analogRead(A3); // Rover is not in motion so current can be zeroed 
        voltage();
        
        
  
      }
      
      
}
//delay(100);


//Serial.print("time ");
//Serial.println(tiempo);
//Serial.println("signals");
// Serial.println(fb);
// Serial.println(rl);
 //delay(100);

currentDraw();
   

 
  
  
}
