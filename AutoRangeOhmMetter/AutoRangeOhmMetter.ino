/*
 Name:		AutoRangeOhmMetter.ino
 Created:	2/14/2018 1:11:09 PM
 Author:	a.nourbakhsh
*/
#include <stdio.h>      

int AnalogPin = 7;  
   
int DigitalPinStep1 = 22;
float KnownResistorStep1 = 100000; //Ohm

int DigitalPinStep2 = 23;
float KnownResistorStep2 = 10000; //Ohm

int DigitalPinStep3 = 24;
float KnownResistorStep3 = 1000; //Ohm

int DigitalPinStep4 = 25;
float KnownResistorStep4 = 100; //Ohm



int ADCMaxValue = 1024;
int ToleranceValue = 0;
int LastTolerance = ADCMaxValue / 2;

int ADCValue = 0;
int LastADCValue = 0;

float InputVoltage = 5;           
float LowSideVoltage = 0;       
float HighSideVoltage = 0; 
float KnownResistor = 0;      
float UnknownResistor = 0;      
float CalcBuffer = 0;          

void setup () 
{ 
  Serial.begin (9600);   
  Serial.println (); 

  pinMode(DigitalPinStep1, OUTPUT);  
  digitalWrite(DigitalPinStep1, LOW);
  
  pinMode(DigitalPinStep2, OUTPUT);  
  digitalWrite(DigitalPinStep2, LOW);

  pinMode(DigitalPinStep3, OUTPUT);  
  digitalWrite(DigitalPinStep3, LOW);

  pinMode(DigitalPinStep4, OUTPUT);  
  digitalWrite(DigitalPinStep4, LOW);  
} 


int GetTolerance(int ADCValue)
{
  if (ADCValue <= (ADCMaxValue / 2)) 
    return (ADCMaxValue / 2) - ADC;
  else
    return ADCValue - (ADCMaxValue / 2);
}

void loop () 
{ 

  //Reset To Default
  LastTolerance = ADCMaxValue / 2;  
  digitalWrite(DigitalPinStep1, LOW);
  digitalWrite(DigitalPinStep2, LOW);
  digitalWrite(DigitalPinStep3, LOW);
  digitalWrite(DigitalPinStep4, LOW);  

  Serial.println("-----");

  delay(10);
  
  //Step1  
  digitalWrite(DigitalPinStep1, HIGH);
  ADCValue = analogRead(AnalogPin);
  Serial.print("AnalogADCRead Step1:"); Serial.println(ADCValue); 
  ToleranceValue = GetTolerance(ADCValue);
  if (ToleranceValue <= LastTolerance)
  { 
    LastADCValue = ADCValue;
    LastTolerance = ToleranceValue;
    KnownResistor = KnownResistorStep1; 
  }
  digitalWrite(DigitalPinStep1, LOW);
  delay(10);

  //Step2  
  digitalWrite(DigitalPinStep2, HIGH);
  ADCValue = analogRead(AnalogPin);
  Serial.print("AnalogADCRead Step2:"); Serial.println(ADCValue); 
  ToleranceValue = GetTolerance(ADCValue);
  if (ToleranceValue <= LastTolerance)
  { 
    LastADCValue = ADCValue;
    LastTolerance = ToleranceValue;
    KnownResistor = KnownResistorStep2; 
  }  
  digitalWrite(DigitalPinStep2, LOW);
  delay(00);

  //Step3  
  digitalWrite(DigitalPinStep3, HIGH);
  ADCValue = analogRead(AnalogPin);
  Serial.print("AnalogADCRead Step3:"); Serial.println(ADCValue); 
  ToleranceValue = GetTolerance(ADCValue);
  if (ToleranceValue <= LastTolerance)
  { 
    LastADCValue = ADCValue;
    LastTolerance = ToleranceValue;
    KnownResistor = KnownResistorStep3; 
  }
  digitalWrite(DigitalPinStep3, LOW);
  delay(10);
  

  //Step4  
  digitalWrite(DigitalPinStep4, HIGH);
  ADCValue = analogRead(AnalogPin);
  Serial.print("AnalogADCRead Step4:"); Serial.println(ADCValue); 
  ToleranceValue = GetTolerance(ADCValue);
  if (ToleranceValue <= LastTolerance)
  { 
    LastADCValue = ADCValue;
    LastTolerance = ToleranceValue;
    KnownResistor = KnownResistorStep4; 
  }
  digitalWrite(DigitalPinStep4, LOW);
  delay(10);

  if (LastADCValue == ADCMaxValue - 1 || LastTolerance >= (ADCMaxValue/2) - 5)
  {
    Serial.println("Canot Measure !!!");
    goto endloop;
  }
    
  Serial.println("Result:");
  Serial.print("KnownResistor(Ohm):"); Serial.println(KnownResistor); 

  //FixAdc
  //LastADCValue = LastADCValue +((LastADCValue * 0) / 100);
  
  LowSideVoltage = ((InputVoltage * LastADCValue) / ADCMaxValue); 
  Serial.print("KnownResistorVoltage:"); Serial.println(LowSideVoltage);  


  CalcBuffer = (InputVoltage / LowSideVoltage) - 1;  
  UnknownResistor = (KnownResistor * CalcBuffer) ;   
  Serial.print("UnknownResistor(Ohm):"); Serial.println(UnknownResistor); 


  HighSideVoltage = (InputVoltage - LowSideVoltage) ;
  Serial.print("UnknownResistorVoltage:"); Serial.println(HighSideVoltage);  
  
  endloop:
  delay (2000);


  }
