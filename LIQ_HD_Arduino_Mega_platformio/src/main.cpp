
#include <Arduino.h>
#include "globals.h"
// #include "sensor_functions.h"
#include "SD_functions.h"
// #include "serial_functions.h"

// Settings settings;

void setup(){
	Serial.begin(115200);
	Serial.println("System ready.");
}

void loop() {
	bool create_log_file();
	// void log_to_SD(signed char sipper_id, unsigned long timestamp, signed char state);
	void close_log_file();
}