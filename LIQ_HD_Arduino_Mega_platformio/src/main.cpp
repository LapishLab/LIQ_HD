#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_MPR121.h>
#include <RTClib.h>
SdFat SD;
RTC_PCF8523 rtc;
char logFileName[32] = "touchlog.csv"; // name of the log file on the SD card
// application notes for MPR121
// https://www.nxp.com/docs/en/application-note/AN3892.pdf

/////////////// Settings you might want to change //////////////////
// Decrease value to increase sensitivity
const int touch_threshold = 120;   // default = 12
const int release_threshold = 120; // default = 6
const int off_debounce = 40;       // time in milliseconds to wait before declaring a lick has stopped (maybe smarter to use sensor's built in debounce? But complicated.)
const int on_debounce = 40;        // time in milliseconds to wait before declaring a lick has started
///////////////////////////////////////////////////////////

const int button1Pin = 48;
const int chipSelect = 7;

const int NUM_SENSORS = 3;
const int PADS_PER_SENSOR = 12;
const uint8_t sensorAddresses[NUM_SENSORS] = {0x5A, 0x5B, 0x5C};
Adafruit_MPR121 caps[NUM_SENSORS];

unsigned long lick_start_time[NUM_SENSORS][PADS_PER_SENSOR];
unsigned long lick_stop_time[NUM_SENSORS][PADS_PER_SENSOR];
bool currently_licking[NUM_SENSORS][PADS_PER_SENSOR];

unsigned long experiment_start_time = 0;

void initialize_variables(){
  for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
  {
    for (uint8_t pad = 0; pad < PADS_PER_SENSOR; pad++)
    {
      lick_start_time[sensor][pad] = 0;
      lick_stop_time[sensor][pad] = 0;
      currently_licking[sensor][pad] = false;
    }
  }
}

void logTouchToSD(int sipper_id, unsigned long timestamp, int state){
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.print(sipper_id);
    dataFile.print(" , ");
    dataFile.print(timestamp);
    dataFile.print(" , ");
    dataFile.println(state);
    dataFile.close();

    Serial.print(sipper_id);
    Serial.print(" , ");
    Serial.print(timestamp);
    Serial.print(" , ");
    Serial.println(state);
  }
  else
  {
    Serial.println("Error opening log file");
  }
}

void write_SD_headers(){
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("sipper_id , timestamp, state");
    dataFile.close();
  }
  else
  {
    Serial.println("Error opening logFileName");
  }
}

void record(){
  initialize_variables();
  int sipper_id;
  DateTime now = rtc.now();
  snprintf(logFileName, sizeof(logFileName), "%04d%02d%02d_%02d%02d%02d.csv", now.year() % 100, now.month(), now.day(), now.hour(), now.minute(), now.second());
  experiment_start_time = millis(); // set the experiment start time to the current time
  write_SD_headers();               // Write the headers to the SD card file
  while (true)
  {
    for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
    {
      uint16_t touched = caps[sensor].touched();            // bit array with 1s and 0s meaning touched vs. not touched for each of 12 sippers
      unsigned long now = millis() - experiment_start_time; // time of recording the touched status (time in milliseconds since the experiment started).
      for (uint8_t pad = 0; pad < PADS_PER_SENSOR; pad++)
      { // loop through each sipper by bitmasking againt shifted 00001

        // Goal - Detect the start of a new lick
        if (touched & (1 << pad))
        { // If this sipper registers as being touched
          if (!currently_licking[sensor][pad])
          { // If this sipper is not already mid-lick
            if (now - lick_stop_time[sensor][pad] > off_debounce)
            {                                             // If sufficient time has passed since the last lick stopped
              currently_licking[sensor][pad] = true;      // We are now considered currently licking
              lick_start_time[sensor][pad] = now;         // We save the time this lick started
              sipper_id = sensor * PADS_PER_SENSOR + pad; // Compute the sipper ID counting from 0 to 35 accross the 3 capacitive touch sensors
              logTouchToSD(sipper_id, now, 1);            // Log the sipper time with state=1, meaning this is lick start
            }
          }
        }
        // Goal - Detect the stop of the current lick
        else
        { // If this sipper registers as NOT being touched
          if (currently_licking[sensor][pad])
          { // If this sipper is mid-lick
            if (now - lick_start_time[sensor][pad] > on_debounce)
            {                                             // If sufficient time has passed since the last lick started
              currently_licking[sensor][pad] = false;     // We are now considered not currently licking
              lick_stop_time[sensor][pad] = now;          // We save the time this lick stopped
              sipper_id = sensor * PADS_PER_SENSOR + pad; // Compute the sipper ID counting from 0 to 35 accross the 3 capacitive touch sensors
              logTouchToSD(sipper_id, now, 0);            // Log the sipper time with state=0, meaning this is lick stop
            }
          }
        }
      }
    }
    delay(20);
  }
}

void set_sensor_settings(){
  int debounce = 0; // 0 = turned off. Complicated bit array setting to set the actual range of touched and released (see application notes)      //range 0-7
  for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
  {
    caps[sensor].setThresholds(touch_threshold, release_threshold); // set sensitivity of touch and release of capacitive sensors
    // cap.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

    caps[sensor].writeRegister(MPR121_DEBOUNCE, debounce);
    caps[sensor].writeRegister(MPR121_MHDR, 1);
    caps[sensor].writeRegister(MPR121_NHDR, 10);
    caps[sensor].writeRegister(MPR121_NCLR, 1);
    caps[sensor].writeRegister(MPR121_FDLR, 2);

    caps[sensor].writeRegister(MPR121_MHDF, 1);
    caps[sensor].writeRegister(MPR121_NHDF, 5);
    caps[sensor].writeRegister(MPR121_NCLF, 3);
    caps[sensor].writeRegister(MPR121_FDLF, 1);

    caps[sensor].writeRegister(MPR121_NHDT, 1);
    caps[sensor].writeRegister(MPR121_NCLT, 5);
    caps[sensor].writeRegister(MPR121_FDLT, 1);
  }
}
void setup(){
  Serial.begin(9600);
  pinMode(button1Pin, INPUT_PULLUP);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (!caps[i].begin(sensorAddresses[i]))
    {
      Serial.print("MPR121 #");
      Serial.print(i + 1);
      Serial.print(" not found at 0x");
      Serial.println(sensorAddresses[i], HEX);
      while (1)
        ;
    }
  }

  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card initialization failed!");
    while (1)
      ;
  }
  Serial.println("System ready.");
}

void loop() {
  if (digitalRead(button1Pin) == LOW) {
    Serial.println("Button 1 pressed. Starting recording...");
    delay(300);
    record();
  }
}