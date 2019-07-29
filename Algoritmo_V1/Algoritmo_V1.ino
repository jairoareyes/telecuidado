#include "teleciudadoLib.h"

#define valvePin D5
#define pumpPin D7
#define analogInPin A0
#define REPORTING_PERIOD_MS     15000

char data;
bool isTakePressureOn = false;

void setup() {
  pinMode(valvePin,OUTPUT);
  pinMode(pumpPin,OUTPUT);
  Serial.begin(115200);
  Serial.println(" ");
  teleLib.initPulOxi();
}

void loop() {
  teleLib.updatePulOxi();
  if ((millis() - teleLib.tsLastReport2 > REPORTING_PERIOD_MS) && teleLib.isPulOxiOn) {
    float BPMProm=0;
    for (int i=0;i<teleLib.nSamples;i++){
      BPMProm += teleLib.BPMArray[i];
    }
    BPMProm = BPMProm/teleLib.nSamples;
    Serial.print("BPM Promedio: ");
    Serial.println(BPMProm);
    teleLib.nSamples=0;
    teleLib.shutdownPulOxi();
    teleLib.isPulOxiOn=false;
  }

  if (Serial.available() > 0) {
    data = Serial.read();

    if (data == '1'){ //Toma Pulso Oximetria
      Serial.println("Taking Pulse Oximeter");
      teleLib.tsLastReport2 = millis(); 
      teleLib.isPulOxiOn=true;
    }
    else if(data == '2'){ // Infla brazalete
      inflateCuff();
    }
    else if(data == '3'){ // Toma la presion
      // poxi.begin();
      // if (!poxi.begin()) {
      //   Serial.println("FAILED");
      // } 
      // else {
      //   Serial.println("SUCCESS");
      // }
      // isTakePressureOn=true;
      takePressure();
    }
    else if (data =='c'){ // Lee el valor de la presion
      float pressureValue=readPressure();
      Serial.print("Presion: ");
      Serial.println(pressureValue);
    }
  }
}

void inflateCuff(){
  float pressureValue = readPressure();
  digitalWrite(valvePin,LOW);
  Serial.println("inflateCuff");
  Serial.println(pressureValue);
  while (pressureValue<120){
    digitalWrite(pumpPin,HIGH);
    pressureValue = readPressure();   
  }
  digitalWrite(pumpPin,LOW);
}

void takePressure(){
  float pressureValue = readPressure();
  Serial.println("Take Pressure");
  while (pressureValue>50){
    digitalWrite(valvePin,HIGH);  
    pressureValue=readPressure();
    Serial.println(pressureValue);
  }
  Serial.println("Finished!");
}

float readPressure(){
  float adcValue=0;
  for (int i=0; i<5; i++){
    adcValue += analogRead(analogInPin); 
    delay(5);    
  }
  adcValue=adcValue/5;
  adcValue = (adcValue - 20.6);
  adcValue = adcValue/2.36;
  return adcValue;
}
