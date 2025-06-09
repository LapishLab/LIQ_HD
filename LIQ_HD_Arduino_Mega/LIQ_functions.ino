//functions used throughout code

//======================================================================================
// SHOULD PRINT
void reset_total_LN() {
  DEBUG_PRINTLN("Ran -  reset_total_LN() ");
  for (int k = 0; k < 36; k++) {
    total_LN[k] = 0;  //reset totals
  }
}

//======================================================================================
// SHOULD PRINT
void reset_variables() {
  DEBUG_PRINTLN("Ran -  reset_variables() ");
  //Reset lick variables
  for (int k = 0; k < 36; k++) {
    LickNumber[k] = 0;
    LickDuration[k] = 0;
    BoutNumber[k] = 0;
    BoutDuration[k] = 0;
    BoutLickNumber[k] = 0;
    BoutLickDuration[k] = 0;
    BoutLickDuration_bytime[k] = 0;
  }
}

//==========================================================================================
// SHOULD PRINT
void Record_Licks() {
  //DEBUG_PRINT("Ran -  Record_Licks()");
  for (int k = 0; k < 12; k++) {  //for cap sensor 1 it loops through all 12 sensors on A
    currtouched1 = cap.touched();
    if ((currtouched1 & _BV(k)) && !licking[k]) {   //if the sensor is touched and if the pad is not marked as licking it will log 1 lick
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" touched");
      cal_timer = millis(); //Start or update timers
      time_now[k] = millis();
      licking[k] = true; //record as new lick
      LickNumber[k] += 1;
      lick_bout_countdown[k] += 1;
      if (lick_bout_countdown[k] == 1) {
        bout_start_timer[k] = millis(); //one lick means start timer 
      }
      if (lick_bout_countdown[k] == 3 && millis() - bout_start_timer[k] <= 1000) { //a bout is 3 licks within 1 second
        BoutNumber[k] += 1;
        BoutLickNumber[k] += 2;  //include first 2 licks in bout total, now that bout is confirmed
        bout_timer[k] = millis();
        in_bout[k] = true;
        DEBUG_PRINT(k);
        DEBUG_PRINT(" bout started!");
      }
      if (in_bout[k]) {  //change this to just if(in_bout && millis() - bout_timer[k] <= 3000)? this would only count licks if they happen within 3 seconds of the last lick
        BoutLickNumber[k] += 1;
        bout_timer[k] = millis();
      }
    }
    if (!(currtouched1 & _BV(k)) && licking[k]) { //detects lick has ended if it was previously touched but now it is not.
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" released");
      last_lick_time[k] = millis();
      Elapsedtime[k] = millis() - time_now[k];
      LickDuration[k] = LickDuration[k] + Elapsedtime[k];
      BoutLickDuration[k] = BoutLickDuration[k] + Elapsedtime[k];
      if (in_bout[k] && lick_bout_countdown[k] == 3) {
        BoutLickDuration_bytime[k] = BoutLickDuration[k];
      }
      if (in_bout[k] && lick_bout_countdown[k] != 3) {  `
        BoutLickDuration_bytime[k] = BoutLickDuration_bytime[k] + Elapsedtime[k]; //separately track duration of licks in bout time window
      }
      licking[k] = false; //this will reset the licking state to then detect another lick
      DEBUG_PRINTLN(LickNumber[k]);
      DEBUG_PRINTLN(LickDuration[k]);
    }
  }
  lasttouched1 = currtouched1;

  for (int k = 12; k < 24; k++) {  //for cap sensor 2
    currtouched2 = cap2.touched();
    int s = k - 12;
    if ((currtouched2 & _BV(s)) && !licking[k]) {
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" touched");
      cal_timer = millis();
      time_now[k] = millis();
      licking[k] = true;
      LickNumber[k] += 1;
      lick_bout_countdown[k] += 1;
      if (lick_bout_countdown[k] == 1) {
        bout_start_timer[k] = millis();
      }
      if (lick_bout_countdown[k] == 3 && millis() - bout_start_timer[k] <= 1000) {
        BoutNumber[k] += 1;
        BoutLickNumber[k] += 2;  //add licks missed before bout started
        bout_timer[k] = millis();
        in_bout[k] = true;
        DEBUG_PRINT(k);
        DEBUG_PRINT(" bout started!");
      }
      if (in_bout[k]) {  //change this to just if(in_bout && millis() - bout_timer[k] <= 3000)?
        BoutLickNumber[k] += 1;
        bout_timer[k] = millis();
      }
    }
    if (!(currtouched2 & _BV(s)) && licking[k]) {
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" released");
      last_lick_time[k] = millis();
      Elapsedtime[k] = millis() - time_now[k];
      LickDuration[k] = LickDuration[k] + Elapsedtime[k];
      BoutLickDuration[k] = BoutLickDuration[k] + Elapsedtime[k];
      if (in_bout[k] && lick_bout_countdown[k] == 3) {
        BoutLickDuration_bytime[k] = BoutLickDuration[k];
      }
      if (in_bout[k] && lick_bout_countdown[k] != 3) {
        BoutLickDuration_bytime[k] = BoutLickDuration_bytime[k] + Elapsedtime[k];
      }
      licking[k] = false;
      DEBUG_PRINTLN(LickNumber[k]);
      DEBUG_PRINTLN(LickDuration[k]);
    }
  }
  lasttouched2 = currtouched2;

  for (int k = 24; k < 36; k++) {  //for cap sensor 3
    currtouched3 = cap3.touched();
    int s = k - 24;
    if ((currtouched3 & _BV(s)) && !licking[k]) {
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" touched");
      cal_timer = millis();
      time_now[k] = millis();
      licking[k] = true;
      LickNumber[k] += 1;
      lick_bout_countdown[k] += 1;
      if (lick_bout_countdown[k] == 1) {
        bout_start_timer[k] = millis();
      }
      if (lick_bout_countdown[k] == 3 && millis() - bout_start_timer[k] <= 1000) {
        BoutNumber[k] += 1;
        BoutLickNumber[k] += 2;  //add licks missed before bout started
        bout_timer[k] = millis();
        in_bout[k] = true;
        DEBUG_PRINT(k);
        DEBUG_PRINTLN(" bout started!");
      }
      if (in_bout[k]) {  //change this to just if(in_bout && millis() - bout_timer[k] <= 3000)?
        BoutLickNumber[k] += 1;
        bout_timer[k] = millis();
      }
    }
    if (!(currtouched3 & _BV(s)) && licking[k]) {
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" released");
      last_lick_time[k] = millis();
      Elapsedtime[k] = millis() - time_now[k];
      LickDuration[k] = LickDuration[k] + Elapsedtime[k];
      BoutLickDuration[k] = BoutLickDuration[k] + Elapsedtime[k];
      if (in_bout[k] && lick_bout_countdown[k] == 3) {
        BoutLickDuration_bytime[k] = BoutLickDuration[k];
      }
      if (in_bout[k] && lick_bout_countdown[k] != 3) {
        BoutLickDuration_bytime[k] = BoutLickDuration_bytime[k] + Elapsedtime[k];
      }
      licking[k] = false;
      DEBUG_PRINTLN(LickNumber[k]);
      DEBUG_PRINTLN(LickDuration[k]);
    }
  }
  lasttouched3 = currtouched3; //stores last touch state for the sensors

  for (int k = 0; k < 36; k++) {
    if (lick_bout_countdown[k] <= 2 && millis() - bout_start_timer[k] >= 1000) { // if less than 3 licks occurred in 1 second, it is invalid, so it will be cleared.
      bout_start_timer[k] = 0;
      lick_bout_countdown[k] = 0;
      if (log_by_bout) {
        BoutLickNumber[k] = 0;
        BoutLickDuration[k] = 0;
      }
    }
    if (lick_bout_countdown[k] >= 3 && millis() - bout_timer[k] >= bout_cutoff) {
      DEBUG_PRINT(k);
      DEBUG_PRINTLN(" bout ended!");
      in_bout[k] = false;
      if (log_by_bout) {
        BoutDuration[k] = last_lick_time[k] - bout_start_timer[k];
        LickFrequency[k] = float(BoutLickNumber[k]) / (float(BoutDuration[k]) / 1000);
        ILI[k] = (float(BoutDuration[k]) - float(BoutLickDuration[k])) / float(BoutLickNumber[k]);
        write_to_file_by_bout(k);
        calc_total_LN();
        BoutDuration[k] = 0;
        BoutLickNumber[k] = 0;
        BoutLickDuration[k] = 0;
      }
      if (log_by_time) {
        BoutDuration[k] = BoutDuration[k] + (last_lick_time[k] - bout_start_timer[k]);
      }
      bout_start_timer[k] = 0;
      lick_bout_countdown[k] = 0;
    }
  }
}

//======================================================================================
// SHOULD PRINT
void update_sippers() {
  DEBUG_PRINT("Ran -  update_sippers() ");
  for (int k = 0; k < 36; k++) {
    if (licking[k]) {
      Elapsedtime[k] = millis() - time_now[k];
      LickDuration[k] = LickDuration[k] + Elapsedtime[k];
      if (in_bout[k]) {
        BoutLickDuration[k] = BoutLickDuration[k] + Elapsedtime[k];
        BoutLickDuration_bytime[k] = BoutLickDuration_bytime[k] + Elapsedtime[k];
      }
    }
    if (in_bout[k]) {
      BoutDuration[k] = BoutDuration[k] + (millis() - bout_start_timer[k]);
    }
  }
}


//======================================================================================

void reset_time_now() {
  for (int k = 0; k < 36; k++) {
    time_now[k] = millis();  //reset time now
    if (in_bout[k]) {
      bout_start_timer[k] = last_lick_time[k];
    }
  }
}

//======================================================================================

void calc_total_LN() {
  if (log_by_time) {
    for (int k = 0; k < 36; k++) {
      total_LN[k] = total_LN[k] + LickNumber[k];  //calculate total LN for each sipper
    }
  }
  if (log_by_bout) {
    for (int k = 0; k < 36; k++) {
      total_LN[k] = LickNumber[k];  //calculate total LN for each sipper
    }
  }
}

//======================================================================================

void wait(unsigned long ms) {
  unsigned long X = millis();
  while (millis() < X + ms) {}  //wait without delay for ms (milliseconds)
}

//======================================================================================

void set_defaults() {
  default_lights_on = lights_on;
  default_lights_off = lights_off;
  default_touch_threshold = touch_threshold;
  default_release_threshold = release_threshold;
  default_LOG_INTERVAL = LOG_INTERVAL;
  dafault_SYNC_INTERVAL = SYNC_INTERVAL;
  default_auto_cal = auto_cal;
  default_auto_cal_time = auto_cal_time;
  default_auto_cal_sec_since_last_lick = auto_cal_sec_since_last_lick;
}

//======================================================================================

void auto_calibration() {
  if (auto_cal) {
    unsigned long temp_cal_time = 1000 * auto_cal_sec_since_last_lick;
    unsigned long temp_cal_timer = millis() - cal_timer;
    now = rtc.now();
    if (auto_cal_flag && now.hour() == auto_cal_time && temp_cal_timer > temp_cal_time) {
      auto_cal_flag = !auto_cal_flag;
      cap.begin(0x5A);
      cap2.begin(0x5B);
      cap3.begin(0x5C);
      set_sensor_settings();
    }
    if (now.hour() != auto_cal_time) {
      auto_cal_flag = !auto_cal_flag;
    }
  }
}

//======================================================================================

void set_sensor_settings() {
  cap.setThresholds(touch_threshold, release_threshold);  //set sensitivity of touch and release of capacitive sensors
  //cap.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

  cap.writeRegister(MPR121_DEBOUNCE, debounce);
  cap.writeRegister(MPR121_MHDR, 1);
  cap.writeRegister(MPR121_NHDR, 10);
  cap.writeRegister(MPR121_NCLR, 1);
  cap.writeRegister(MPR121_FDLR, 2);

  cap.writeRegister(MPR121_MHDF, 1);
  cap.writeRegister(MPR121_NHDF, 5);
  cap.writeRegister(MPR121_NCLF, 3);
  cap.writeRegister(MPR121_FDLF, 1);

  cap.writeRegister(MPR121_NHDT, 1);
  cap.writeRegister(MPR121_NCLT, 5);
  cap.writeRegister(MPR121_FDLT, 1);

  cap2.setThresholds(touch_threshold, release_threshold);  //set sensitivity of touch and release of capacitive sensors
  //cap2.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

  cap2.writeRegister(MPR121_DEBOUNCE, debounce);
  cap2.writeRegister(MPR121_MHDR, 1);
  cap2.writeRegister(MPR121_NHDR, 10);
  cap2.writeRegister(MPR121_NCLR, 1);
  cap2.writeRegister(MPR121_FDLR, 2);

  cap2.writeRegister(MPR121_MHDF, 1);
  cap2.writeRegister(MPR121_NHDF, 5);
  cap2.writeRegister(MPR121_NCLF, 3);
  cap2.writeRegister(MPR121_FDLF, 1);

  cap2.writeRegister(MPR121_NHDT, 1);
  cap2.writeRegister(MPR121_NCLT, 5);
  cap2.writeRegister(MPR121_FDLT, 1);

  cap3.setThresholds(touch_threshold, release_threshold);  //set sensitivity of touch and release of capacitive sensors
  //cap3.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

  cap3.writeRegister(MPR121_DEBOUNCE, debounce);
  cap3.writeRegister(MPR121_MHDR, 1);
  cap3.writeRegister(MPR121_NHDR, 10);
  cap3.writeRegister(MPR121_NCLR, 1);
  cap3.writeRegister(MPR121_FDLR, 2);

  cap3.writeRegister(MPR121_MHDF, 1);
  cap3.writeRegister(MPR121_NHDF, 5);
  cap3.writeRegister(MPR121_NCLF, 3);
  cap3.writeRegister(MPR121_FDLF, 1);

  cap3.writeRegister(MPR121_NHDT, 1);
  cap3.writeRegister(MPR121_NCLT, 5);
  cap3.writeRegister(MPR121_FDLT, 1);
}
