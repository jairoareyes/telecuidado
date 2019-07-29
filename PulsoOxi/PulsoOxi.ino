#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     6000

PulseOximeter pox;
uint32_t tsLastReport2 = 0;
uint8_t nSamples2 = 0;
float BPMArray[10];
float BPMProm=0;
void setup()
{
    Wire.begin(4,5);
    pinMode(LED_BUILTIN,OUTPUT);
    Serial.begin(115200);
    Serial.println(" ");
    Serial.print("Initializing pulse oximeter.. ");

    pox.begin();
    if (!pox.begin()) {
        Serial.println("FAILED");
        //for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    //pinMode(13,OUTPUT);
    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

    if (millis() - tsLastReport2 > REPORTING_PERIOD_MS) {
        BPMProm=0;
        for (int i=0;i<nSamples2;i++){
            BPMProm += BPMArray[i];
        }
        BPMProm = BPMProm/nSamples2;
        Serial.print("BPM Promedio: ");
        Serial.println(BPMProm);
        nSamples2=0;
        tsLastReport2 = millis();
    }
}

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{   
    BPMArray[nSamples2] = pox.getHeartRate();
    // Serial.println("Beat!");
    Serial.print("Heart rate:");
    Serial.print(BPMArray[nSamples2]);
    Serial.print("bpm / SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    nSamples2++;
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
}

// void calculateBPM(){
//     double BPMs;
//     double period = (millis()-tsLastReport);
//     period = period /60000;
//     BPMs=10/(period);
//     Serial.print("BPMs Calculado: ");
//     Serial.println(BPMs);
//     nSamples = 0;
// }
