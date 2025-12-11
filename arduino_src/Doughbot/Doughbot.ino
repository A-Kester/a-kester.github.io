#include <Wire.h>
#include "Motor.h"
#include "Display.h"
#include "Buttons.h"
#include "TOF.h"

#define MAX_TURNS 36
#define STEP_MM 3
#define TEN_MIN_MS 600000
#define ONE_MIN_MS 60000

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
uint8_t progress = 0;

uint8_t val_array_len = 0;
uint32_t bowl_area = 846333;
uint32_t dough_area = 0;
uint32_t init_dough_area = 0;

unsigned long start_time = 0;
unsigned long target_time = 7200000; // 2 hours in ms
struct temp_humid th_struct;

double riemannSum();
double getVolume(int* vals);
uint32_t scan_bowl();
uint32_t get_area();
void unwind_motor();
void handle_settings_buttons();
void handle_settings_start();

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
  get_temp_humid(&th_struct);
}

void loop() {
  
  unsigned long curr;
  uint8_t scan_freq = 0;
  switch (state) {
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
      handle_settings_dir(B_UP);
      handle_settings_dir(B_DOWN);
      if (has_been_pressed(B_START)) {
        Serial.println(settings, BIN);
        handle_settings_start();
      }
      break;
    case 2:
      // Home
      display_home_screen(th_struct.temp);
      if (has_been_pressed(B_START)) {
        prev_state = state; 
        get_temp_humid(&th_struct);
        bowl_area = scan_bowl();
        state = 3;
      } else if (has_been_pressed(B_SWAP)) {
        // If both pressed, probably wanna 
        prev_state = state;
        state = 1;
      }
      break;
    case 3:
      display_ready_screen(th_struct.temp);
      if (has_been_pressed(B_START)) {
        prev_state = state;
        init_dough_area = bowl_area - scan_bowl();
        progress = 0;
        start_time = millis();
        state = 4;
      } else if (has_been_pressed(B_SWAP)) {
        // Go to settings
        prev_state = state;
        state = 1;
      }
      break;
    case 4:
      curr = millis();
      // TEMP
      // 5 is the index for the time vs vol check
      if ((settings & (1 << 5)) != 0) {
        // Using volume rise
        scan_freq = ascii_to_int(*s_freq);
        if ((curr - start_time) >= (TEN_MIN_MS * scan_freq) ) {
          dough_area = bowl_area - scan_bowl();
          //dough_area = bowl_area - 700388;
          progress = (dough_area* 10) / (2 * init_dough_area);
          start_time = curr;
          if (progress >= 100) {
            state = 2;
          }
        }
      } else {
        //Serial.print("checking time: "); Serial.print(curr - start_time); Serial.print(" / "); Serial.println(target_time);
        progress = ((curr - start_time) * 10)/target_time;
        if ((curr - start_time) >= target_time) {
          // time has expired
          state = 2;
        }
      }
      // Check for show progress setting
      if ((settings & (1 << 1)) == 0) {
        display_rise_progress(progress, false);
      } else {
        display_rise_progress(progress, true);
      }
      break;
    case 5:
      Serial.println("Timer expired");
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
    sum += (mil) * STEP_MM * (STEP_MM * i);
    delay(250);
  } 
  Serial.print("Area (mm^2): "); 
  Serial.println(sum);
  unwind_motor();
  return sum;
}

// Takes time as String of format "HH:MM"
unsigned long time_to_millis(char * c) {
  uint8_t hours = (ascii_to_int(*c) * 10) + ascii_to_int(*(c + 1));
  uint16_t minutes = (hours * 60) + (ascii_to_int(*(c + 3)) * 10) + ascii_to_int(*(c + 4));
  return minutes * 60000;
}

void handle_settings_dir(uint8_t button) {
  uint8_t dir = (button == B_DOWN)? DOWN: UP;
  if (has_been_pressed(button)) {
        if (editing_time == 1) {
          int_setting_change(dir, settings_opts_lut[3], time_cursor_lut[time_index]);
        } else if (editing_rise_factor == 1){
          int_setting_change(dir, settings_opts_lut[4], 0);
        } else if (editing_scan_freq == 1){
          int_setting_change(dir, settings_opts_lut[5], 0);
        } else if (settings_index < 7 && dir == DOWN){
          settings_index += 1;
        }else if (settings_index != 0 && dir == UP) {
          settings_index -= 1;
        }
        Serial.println(settings, BIN);
        return;
  }
}

void handle_settings_start() {
  Serial.println(settings_index);
  if (settings_index == 2) {
    settings ^= (1 << (7-settings_index));
    if ((settings & (1 << (7-settings_index))) != 0) {
      Serial.println("VOL selected");
      settings &= 0b11101111;
      settings |= 0b00001100;
      //Serial.println((settings & (1 << (7-settings_index))) != 0));
    }else {
      Serial.println("TIME selected");
      settings |= 0b00010000;
      settings &= 0b11110011;
    }
  } else if (settings_index == 3) {
    if (editing_time == 0) {
        time_index = 0;
        lcd.blink();
      } else {
        time_index++;
      }
      if (time_index == 4) {
        time_index = 0;
        editing_time = 0;
        target_time = time_to_millis(r_time);
        Serial.print("New target time: "); Serial.println(target_time);
        lcd.noBlink();
      } else {
        editing_time = 1;
      }
  } else if (settings_index == 4) {
    if (editing_rise_factor == 0) {
        editing_rise_factor = 1;
        lcd.blink();
        time_index = 0;
      } else {
        editing_rise_factor = 0;
        lcd.noBlink();
      }
  } else if (settings_index == 5) {
      if (editing_scan_freq == 0) {
          editing_scan_freq = 1;
          lcd.blink();
          time_index = 0;
        } else {
          editing_scan_freq = 1;
          lcd.noBlink();
        }
  } else {
    settings ^= (1 << (7-settings_index));
  }
}

void unwind_motor() {
  for (uint8_t i = 0; i < (MAX_TURNS/3); i++ ) {
    myMotor->step(15, BACKWARD, DOUBLE);
    delay(50);
  }
  myMotor->release();
}