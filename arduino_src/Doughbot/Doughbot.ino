// Motor Includes
#include <Wire.h>
#include <Adafruit_MotorShield.h>
// Adafruit seems to think that we need this but its not being found.
//#include "utility/Adafruit_PWMServoDriver.h"

// TEMP/HUMID Includes
#include <Adafruit_AHTX0.h>
// Screen Library:
#include <LiquidCrystal.h>

#define MOTOR_STEPS 200
#define DEFUALT_RPM 1

// 4 chars: 3 digits and a percent sign
#define ROW_SIZE 20
#define PERC_0 0
#define PERC_20 1
#define PERC_40 2
#define PERC_60 3
#define PERC_80 4
#define PERC_100 5

// Set up buttons pins
  // 21 => closest to screen
  // 17 => 2nd
  // 16 => 3rd
  // 19 => 4th
#define B_SWAP 21
#define B_DOWN 16
#define B_UP 17
#define B_START 19

#define SWAP_MASK (1 << 0)
#define UP_MASK (1 << 1)
#define DOWN_MASK (1 << 2)
#define START_MASK (1 << 3)

#define NUM_OPTIONS 8
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

// 
const int rs = 14, en = 32, d4 = 15, d5 = 33, d6 = 27, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t block_blank[] = { // Empty block
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t block_20[] = { // 20% filled
  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};
uint8_t block_40[] = { // 40% filled
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18
};
uint8_t block_60[] = { // 60% filled
  0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C
};
uint8_t block_80[] = { // 80% filled
  0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E
};
uint8_t block_full[] = { // Fully filled block
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
};

uint8_t settings = 0b10101010; 
//Bits (0 is lsb): 
// 7: Wireless
// 6: Alerts
// 5: Rise type
// 4: Rise time
// 3: Rise Factor
// 2: Scan Frequency
// 1: Show Progress Bar
// 0: Show Elapsed Time
char* settings_lut[] = {"WIRELESS: ",  "ALERTS: ", "RISE TYPE: ", "RISE TIME: ", "RISE FACTOR: ", "SCAN FREQ: ", "PROGRESS: ", "ELAPSED TIME: ", "  "};
// Custom option Index's: 3, 4, 5
// Use +8
uint8_t* time_array[] = {0,0,0,0};
uint8_t rise_fac = 2;
uint8_t scan_frec = 4;
uint8_t updating_non_bool = 0;
char* settings_opts_lut[] = {"ON",  "On", "VOL", "00:00", "4",  "5",  "Show", "Show", "  ",
                             "OFF", "OFF","TIME", "NA",   "NA", "NA", "Hide", "Hide", "  "};
uint8_t button_status = 0;
uint8_t settings_index = 0;
uint8_t state = 2;
uint8_t prev_state = 2;
unsigned long last_button_press = 0;
Adafruit_StepperMotor* motor;
Adafruit_AHTX0 aht;
struct temp_humid {
  float temp;
  float humid;
};

void display_home_screen();
void display_ready_screen();
void display_settings(uint8_t sel_index);
void write_prog_str (uint8_t perc, uint8_t row, uint8_t col);
void display_rise_progress(uint8_t perc);
void init_temp_humid();
void get_temp_humid(struct temp_humid *);

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

  pinMode(B_SWAP, INPUT_PULLDOWN);
  pinMode(B_DOWN, INPUT_PULLDOWN);
  pinMode(B_UP, INPUT_PULLDOWN);
  pinMode(B_START, INPUT_PULLDOWN);
  
  attachInterrupt(digitalPinToInterrupt(B_SWAP), swap_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_DOWN), up_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_UP), down_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_START), start_isr, RISING);
  
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.createChar(0, block_blank);
  lcd.createChar(1, block_20);
  lcd.createChar(2, block_40);
  lcd.createChar(3, block_60);
  lcd.createChar(4, block_80);
  lcd.createChar(5, block_full);
}

void loop() {
  // put your main code here, to run repeatedly:
  // To read temp/humid
  //struct temp_humid th_struct;
  //get_temp_humid(&th_struct);
  //
  
  switch (state) {
    case 0:
      // Loading:
      break;
    case 1: 
      // Settings: 
      display_settings(settings_index);
      if (has_been_pressed(B_SWAP)) {
        // Return to the screen we left from!
        state = prev_state;
        prev_state = 1;
        lcd.noCursor();
        lcd.noBlink();
      }
      
      if (has_been_pressed(B_DOWN)) {
        if (updating_non_bool) {
          break;
        }
        if (settings_index != 0) settings_index -= 1;
      }
      if (has_been_pressed(B_UP)) {
        if (settings_index < 7) settings_index += 1;
      }
      // This is stupid, but we cant use bit and because it conflicts...
      if (has_been_pressed(B_START + B_SWAP)) {
        // Trying to modify a custom field
        updating_non_bool = 1;
      }
      if (has_been_pressed(B_START)) {
        settings ^= (1 << (7-settings_index));
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
  
  
  //has_been_pressed(B_SWAP);
  //has_been_pressed(B_DOWN);
  //has_been_pressed(B_UP);
  
  // Bad blocking debounce (works pretty well)
  delay(350);
}

void start_isr() {
  button_status |= START_MASK;
}

void up_isr() {
  button_status |= UP_MASK;
}

void down_isr() {
  button_status |= DOWN_MASK;
}

void swap_isr() {
  button_status |= SWAP_MASK;
}

uint8_t has_been_pressed(uint8_t button) {
  switch(button) {
    case B_SWAP:
      if ((button_status & SWAP_MASK) > 0) {
        // Handle swap button Pressed
        //Serial.println("SWAP");
        button_status &= ~SWAP_MASK; // clear the start bit
        return 1;
      }
      break;
    case B_DOWN:
      if ((button_status & UP_MASK) > 0) {
      // Handle up button Pressed
        //Serial.println("Up");
        button_status &= ~UP_MASK; // clear the start bit
        return 1;
      }
      break;
    case B_UP:
      if ((button_status & DOWN_MASK) > 0) {
        //Serial.println("Down");
        // Handle Down button Pressed
        button_status &= ~DOWN_MASK; // clear the start bit
        return 1;
      }
      break;
    case B_START:
      if ((button_status & START_MASK) > 0) {
        //Serial.println("Start");
        // Handle Start button Pressed
        button_status &= ~START_MASK; // clear the start bit
        return 1;
      }
      break;
    case (B_START + B_SWAP):
      if ((button_status & START_MASK) > 0 && (button_status & SWAP_MASK) ) {
        //Serial.println("Start");
        // Handle Start button Pressed
        button_status &= ~(START_MASK & SWAP_MASK); // clear the start bit
        return 1;
      }
      break;
    default:break;
  }
  return 0;
}

void init_temp_humid(){
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
}

void get_temp_humid(struct temp_humid * s) {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  s->temp = temp.temperature;
  s->humid = temp.relative_humidity;
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
}

void display_home_screen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOUGHBOT: IDLE");
  lcd.setCursor(0, 1);
  lcd.print("INSERT EMPTY BOWL"); 
  lcd.setCursor(0, 2);
  lcd.print("PRESS START TO ZERO");
  lcd.setCursor(0,3);
  lcd.print("Config: ");
}

void display_ready_screen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOUGHBOT: READY");
  lcd.setCursor(0, 1);
  lcd.print("BOWL ZERO'D PRESS");
  lcd.setCursor(0, 2);
  lcd.print("START TO BEGIN");
  lcd.setCursor(0,3);
  lcd.print("Config: ");
}

void display_rise_progress(uint8_t perc) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOUGHBOT: RISING");
  write_prog_str(perc, 0, 1);
  char str_buff[15]; // max value would be 100%'\0' = 5 chars
  sprintf(str_buff, "PROGRESS: %d%%", perc);
  lcd.setCursor(0, 2);
  lcd.print(str_buff);
  lcd.setCursor(0, 3);
  lcd.print("ETC: 00:00");
}

void display_settings(uint8_t sel_index) {
  uint8_t page = sel_index / 3; // Intentional Int division
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SETTINGS: ");
  lcd.print((page + 1));
  lcd.print("/3");

  uint8_t index = (page * 3);
  lcd.setCursor(0, 1);
  lcd.print(settings_lut[index]);
  uint8_t offset = (settings & (1 << (7 - index))) ? 0 : NUM_OPTIONS + 1;
  lcd.print(settings_opts_lut[index + offset]);
 
  index = (page*3) + 1;
  lcd.setCursor(0, 2);
  lcd.print(settings_lut[index]);
  offset = (settings & (1 << (7 - index))) ? 0 : NUM_OPTIONS + 1;
  lcd.print(settings_opts_lut[index + offset]);

  index = (page*3) + 2;
  lcd.setCursor(0, 3);
  lcd.print(settings_lut[(page* 3) + 2]);
  offset = (settings & (1 << (7 - index))) ? 0 : NUM_OPTIONS + 1;
  lcd.print(settings_opts_lut[index + offset]);


  lcd.cursor();
  lcd.blink();
  lcd.setCursor(strlen(settings_lut[sel_index]), (sel_index % 3) + 1);
}


void print_uint8_bin(uint8_t* u, char* str) {
  for (uint8_t i = 7; i >= 0; i-- ) {
    //str[7-i] = ((u & (1<<i)) >> i) == 1 ? '1' : '0';
  }
  str[8] = '/0';
}

void write_prog_str (uint8_t perc, uint8_t col, uint8_t row) {
  // Ensure that perc is at most 100
  if (perc > 100) perc = 100;
  uint8_t index = 0;
  lcd.setCursor(col, row);
  // Set the majority of the prograss bar
  while (perc >= 5) {
    lcd.write(PERC_100);
    perc -= 5;
     index++;
  }
  // Sets the last part of the bar
  switch (perc) {
    case 0:
      // not sure why this one complained
      lcd.write(uint8_t(PERC_0));
      break;
    case 1:
      lcd.write(PERC_20);
      break;
    case 2:
      lcd.write(PERC_40);
      break;
    case 3:
      lcd.write(PERC_60);
      break;
    case 4:
      lcd.write(PERC_80);
      break;
    case 5:
      lcd.write(PERC_100);
      break;
  }
   // Clear the rest of the screen;
  for (;index < 20; index++) {
    lcd.write(" ");
  }
}

