#define valvePin D5
#define pumpPin D7

char data;
int n=0;
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

float arrayADC [100];

int sensorValue = 0;  // value read from the pot
float outputValue, presion;

  
void setup() {
  pinMode(valvePin,OUTPUT);
  pinMode(pumpPin,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    data = Serial.read();

    if (data == 'a'){
      digitalWrite(valvePin,LOW);
      Serial.println("recibida a");
      digitalWrite(pumpPin,HIGH);
      delay(500);
      digitalWrite(pumpPin,LOW);
    }
    else if(data == 'b'){
      digitalWrite(valvePin,HIGH);
      delay(500);
      n=0;
      Serial.println("recibida b");
      while (n<500){
        sensorValue=0;
        presion=0;
        for (int i=0; i<5; i++){
          sensorValue += analogRead(analogInPin);
          delay(10);    
        }
        sensorValue=sensorValue/5;
        presion = (sensorValue - 20.6);
        presion = presion/2.36;
        Serial.println(presion);
        n++;
      }
      digitalWrite(valvePin,LOW);

    }
    else if (data =='c'){
      sensorValue=0;
      for (int i=0; i<100; i++){
        sensorValue += analogRead(analogInPin);
        delay(10);    
        }
      sensorValue=sensorValue/100;
      presion = (sensorValue - 20.6);
      presion = presion/2.36;
      Serial.print("Sensor value: ");
      Serial.print(sensorValue);
      Serial.print(" | Presion: ");
      Serial.println(presion);
    }
  }
}
