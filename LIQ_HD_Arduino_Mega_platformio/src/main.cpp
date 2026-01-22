
#include <Arduino.h>

#define chipSelect 4
// #include "sensor_functions.h"
#include <SdFat.h>
// #include "serial_functions.h"

// Settings settings;

void setup(){
	Serial.begin(115200);
	delay(6000);
	Serial.println("System ready.");
}

void loop() {
	Serial.println("going_to_create_log_file");
		Serial.println("Should be starting the create_log_file function");
	
		SdFat SD;
	if (!SD.begin(chipSelect)) {
		Serial.println("SD card initialization failed!");
	}

	char logFileName[40];
	snprintf(logFileName, sizeof(logFileName),"licks.csv");

	File dataFile;
	dataFile = SD.open(logFileName, FILE_WRITE);
	if (!dataFile) {
		Serial.print("Error creating "); Serial.println(logFileName);
	}

	Serial.print("Writing headers to:"); Serial.println(logFileName);
	dataFile.println("sipper_id,timestamp,state");


	// void log_to_SD(signed char sipper_id, unsigned long timestamp, signed char state);
		if (dataFile) {
		dataFile.close();
		Serial.println("Log file closed.");
	} else {
		Serial.println("No log file to close.");
	}

	Serial.println("going_to_cloe_log_file");

	int c = 0;
	while (true){
		Serial.println(c);
		delay(1000);
		c++;
	}
}