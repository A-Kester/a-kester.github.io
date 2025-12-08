
#include <Wire.h>
#include "Motor.h"
#include "Display.h"
#include "Buttons.h"
#include "TH_sensor.h"
#include "TOF.h"

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

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  myMotor->setSpeed(30);
  init_temp_humid();

  buttons_init();
  lcd_init();
  TOF_init();
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
        myMotor->step(50, FORWARD, SINGLE);
      }
      if (has_been_pressed(B_UP)) {
        myMotor->step(50, BACKWARD, SINGLE);
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



