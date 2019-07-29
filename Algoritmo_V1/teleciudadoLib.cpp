#include "teleciudadoLib.h"

PulseOximeter pox;
uint8_t nSamples = 0;
uint8_t validData = 0;
float BPMArray[20];
uint8_t SPO2Array[20];
bool isPulOxiOn = false;
bool isCalibrate = true;
uint32_t tsLastReport2 = 0;

teleciudadoLib::teleciudadoLib(){
	nSamples = 0;
	validData = 0;
	BPMArray[20];
	isPulOxiOn = false;
	isCalibrate = true;
	tsLastReport2 = 0;
}

void teleciudadoLib::printBPMOXI(float bpm, uint8_t spo2){
    Serial.print("Heart rate:");
    Serial.print(bpm);
    Serial.print("bpm / SpO2: ");
    Serial.print(spo2);
    Serial.println("%");
}

void teleciudadoLib::initPulOxi() {
	Serial.print("Initializing pulse oximeter.. ");
	pox.begin();
	if (!pox.begin()) {
	Serial.println("FAILED");
	} 
	else {
	Serial.println("SUCCESS");
	}
	pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
	pox.setOnBeatDetectedCallback(onBeatDetected);
}

void teleciudadoLib::updatePulOxi(){
	pox.update();
}

void teleciudadoLib::shutdownPulOxi(){
	pox.shutdown();
}

void teleciudadoLib::onBeatDetected(){
  //float pressureValue = readPressure();
  if (teleLib.isCalibrate){
    teleLib.calibratingPulOxi();
  }
  else if (teleLib.isPulOxiOn){
    teleLib.BPMArray[teleLib.nSamples] = pox.getHeartRate();
	teleLib.SPO2Array[teleLib.nSamples] = pox.getSpO2();
    teleLib.printBPMOXI(teleLib.BPMArray[teleLib.nSamples],teleLib.SPO2Array[teleLib.nSamples]);
    teleLib.nSamples++;
  }
  // else if (isTakePressureOn){
  //   Serial.print("Heart rate:");
  //   Serial.print(pox.getHeartRate());
  //   Serial.print("bpm / SpO2: ");
  //   Serial.print(pox.getSpO2());
  //   Serial.print("% | Pressure: ");
  //   Serial.println(pressureValue);
  // }
  else {
    teleLib.printBPMOXI(pox.getHeartRate(),pox.getSpO2());
  }
}

void teleciudadoLib::calibratingPulOxi(){
  BPMArray[nSamples] = pox.getHeartRate();
  SPO2Array[nSamples] = pox.getSpO2();
  nSamples++;
  if (nSamples<2) { // Entra en las dos primeras muestras
    printBPMOXI(BPMArray[nSamples-1],SPO2Array[nSamples-1]);
  }
  else {
    printBPMOXI(BPMArray[nSamples-1],SPO2Array[nSamples-1]);
    if ((abs(BPMArray[nSamples-1]-BPMArray[nSamples-2])<5) && 
		(SPO2Array[nSamples-1]<100)){ // Si la diferencia entre el actual y el anterior es menor que 5
      validData++; // Aumenta el dato valido
      Serial.println("Valid data");
    }
    else {
      validData=0; // Resetea dato valido
      Serial.println("Invalid data");
    }
  }
  if (validData>4){ //Si hubo 5 datos validos consecutivos
    Serial.println("Taking Pulse Oximeter");
    isPulOxiOn = true; // Activa el muestreo
    isCalibrate = false; // Desactiva el modo calibracion
    tsLastReport2 = millis(); // Pone en cero el tiempo de inicio de la muestra
    for (int i=0;i<nSamples;i++){ // limpia el arreglo de muestras
      BPMArray[i]=0;
      nSamples=0; // Resetea el numero de muestras
    }
  }
}

teleciudadoLib teleLib = teleciudadoLib(); // crea instancia para el usuario
