#ifndef teleciudadoLib_h
#define teleciudadoLib_h
 
#include "Arduino.h"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

class teleciudadoLib
{
	public:
		uint8_t nSamples;
		uint8_t validData;
		float BPMArray[20];
		uint8_t SPO2Array[20];
		bool isPulOxiOn;
		bool isCalibrate;
		uint32_t tsLastReport2;
		teleciudadoLib();
		void printBPMOXI(float bpm, uint8_t spo2);
		void initPulOxi();
		void updatePulOxi();
		void shutdownPulOxi();
		void calibratingPulOxi();
	private:
		static void onBeatDetected();
		// void beatCallback();
};
 
#ifdef teleLib
#undef teleLib // Define Objeto llamado myExpulsion
#endif

extern teleciudadoLib teleLib;

#endif