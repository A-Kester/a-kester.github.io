
#include <Wire.h>
#include "Motor.h"
#include "Display.h"
#include "Buttons.h"
#include "TH_sensor.h"
#include "TOF.h"

#define MAX_TURNS 36
#define STEP_MM 3
// Setp size = 3mm


uint8_t rise_fac = 2;
uint8_t scan_frec = 4;
uint8_t updating_non_bool = 0;

uint8_t settings_index = 0;
uint8_t time_cursor_lut[] = {0,1,3,4};
uint8_t time_index = 0;
uint8_t editing_time = 0;
uint8_t editing_rise_factor = 0;
uint8_t editing_scan_freq = 0;
uint8_t state = 2;
uint8_t prev_state = 2;

uint8_t val_array_len = 0;
int bowl_min = 0;

double riemannSum();
double getVolume(int* vals);
uint32_t scan_bowl();
uint32_t zero_bowl();
void unwind_motor();


void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    
  } else {
    Serial.println("Motor Shield found.");
    myMotor->setSpeed(30);
    //init_temp_humid();
  }
 
  
  buttons_init();
  lcd_init();
  TOF_init();
  init_temp_humid();
  Serial.println("Getting Measurments");
  getMeasurements();
}

void loop() {
  // To read temp/humid
  //struct temp_humid th_struct;
  //get_temp_humid(&th_struct);
  
  switch (state) {
    case 0:
      // Loading:
      break;
    case 1: 
      // Settings: 
      display_settings(settings_index, time_cursor_lut[time_index]);
      if (has_been_pressed(B_SWAP)) {
        // Return to the screen we left from!
        state = prev_state;
        prev_state = 1;
        lcd.noCursor();
        lcd.noBlink();
      }
      
      if (has_been_pressed(B_DOWN)) {
        if (editing_time == 1) {
          int_setting_change(DOWN, settings_opts_lut[3], time_cursor_lut[time_index]);
          break;
        } else if (editing_rise_factor == 1){
          int_setting_change(DOWN, settings_opts_lut[4], 0);
          break;
        } else if (editing_scan_freq == 1){
          int_setting_change(DOWN, settings_opts_lut[5], 0);
          break;
        }
        if (settings_index < 7) settings_index += 1;
      }
      if (has_been_pressed(B_UP)) {
        if (editing_time == 1) {
          int_setting_change(UP, settings_opts_lut[3], time_cursor_lut[time_index]);
          break;
        } else if (editing_rise_factor == 1){
          int_setting_change(UP, settings_opts_lut[4], 0);
          break;
        } else if (editing_scan_freq == 1){
          int_setting_change(UP, settings_opts_lut[5], 0);
          break;
        }
        if (settings_index != 0) settings_index -= 1;
      }
      if (has_been_pressed(B_START)) {
        if (settings_index == 3) {
          if (editing_time == 0) {
            time_index = 0;
            lcd.blink();
          } else {
            time_index++;
          }
          if (time_index == 4) {
            time_index = 0;
            editing_time = 0;
            lcd.noBlink();
          } else {
            editing_time = 1;
          }
        } else if (settings_index == 4) {
          if (editing_rise_factor == 0) {
            editing_rise_factor = 1;
            lcd.blink();
            time_index = 0;
          }
          break;
        } else if (settings_index == 5) {
            if (editing_scan_freq == 0) {
            editing_scan_freq = 1;
            lcd.blink();
            time_index = 0;
          }
          break;
        }else {
          settings ^= (1 << (7-settings_index));
          if (settings_index == 2) {
            Serial.println((settings & (1 << (7-settings_index))), BIN);
            if ((settings & (1 << (7-settings_index))) != 0) {
                Serial.println("VOL selected");
                settings &= 0b11101111;
                settings |= 0b00001100;
            }else {
                Serial.println("TIME selected");
                settings |= 0b00010000;
                settings &= 0b11110011;
            }
          }
        }
      }
      break;
    case 2:
      // Home
      display_home_screen();
      if (has_been_pressed(B_DOWN)) {
        zero_bowl();
      }
      if (has_been_pressed(B_UP)) {
        myMotor->step(5, FORWARD, DOUBLE);
      }
      if (has_been_pressed(B_START)) {
        prev_state = state;
        state = 3;
      } else if (has_been_pressed(B_SWAP)) {
        // If both pressed, probably wanna 
        prev_state = state;
        state = 1;
      }
      break;
    case 3:
      display_ready_screen();
      if (has_been_pressed(B_START)) {
        prev_state = state;
        state = 4;
      } else if (has_been_pressed(B_SWAP)) {
        // If both pressed, probably wanna 
        prev_state = state;
        state = 1;
      }
      break;
    case 4:
      display_rise_progress(45);
      // TEMP
      delay(5000);
      state = 2;
      break;
  }
  // Bad blocking debounce (works pretty well)
  delay(350);
}

uint32_t scan_bowl() {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < MAX_TURNS; i++ ) {
    myMotor->step(5, FORWARD, DOUBLE);
    myMotor->release();
    int mil = get_mil();
    if (mil == -1) {
      return sum; 
    }
    if (i == 0) {
      bowl_min = mil;
    }
    //bowl_min - mil
    sum += (mil) * STEP_MM;
    delay(250);
  } 
  Serial.print("volume (mm^3): "); 
  Serial.println(sum);
  return sum;
}

uint32_t scan_dough() {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < MAX_TURNS; i++ ) {
    myMotor->step(5, FORWARD, DOUBLE);
    myMotor->release();
    int mil = get_mil();
    if (mil == -1) {
      return sum; 
    }
    if (i == 0) {
      bowl_min = mil;
    }
    //bowl_min - mil
    sum += (bowl_min);
    delay(250);
  } 
  Serial.print("volume (mm^3): "); 
  Serial.println(sum);
  return sum;
}

void unwind_motor() {
  for (uint8_t i = 0; i < (MAX_TURNS/3); i++ ) {
    myMotor->step(15, BACKWARD, DOUBLE);
    delay(50);
  }
  myMotor->release();
}

uint32_t zero_bowl () {
  uint32_t bowl_slice = scan_bowl();
  unwind_motor();
  return bowl_slice;
}

double riemannSum() {
  top = -1;
  //Serial.print("In riemann: ");
  double sum = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    sum += tof_data[i] * step;  // Riemann Sum of area under the curve
    //Serial.println(tof_data[i]);
  }
  return sum;
}

double getVolume(int* vals) {
  double volume = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    volume += i;
  }
  volume *= 2 * M_PI * step;
  volume = volume * (bowl_area - dough_area); // multiply area btwn. the curves
  return volume;
}
