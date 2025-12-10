#include <LiquidCrystal.h>
#include "Helper.h"

// 4 chars: 3 digits and a percent sign
#define ROW_SIZE 20
#define PERC_0 0
#define PERC_20 1
#define PERC_40 2
#define PERC_60 3
#define PERC_80 4
#define PERC_100 5

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
char r_time[] = {'0', '2', ':', '0', '0', '\0'};
char r_factor[] = {'2', '\0'};
char s_freq[] = {'1', '0', '\0'};
char * settings_opts_lut[] = {"ON",  "On", "VOL", r_time, r_factor,  s_freq,  "Show", "Show", "  ",
                             "OFF", "OFF","TIME", "NA",   "NA", "NA", "Hide", "Hide", "  "};

void lcd_init();
void display_home_screen();
void display_ready_screen();
void display_rise_progress(uint8_t perc);
void display_settings(uint8_t sel_index, uint8_t cursor_offset);
void write_prog_str (uint8_t perc, uint8_t row, uint8_t col);

void lcd_init() {
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.createChar(0, block_blank);
  lcd.createChar(1, block_20);
  lcd.createChar(2, block_40);
  lcd.createChar(3, block_60);
  lcd.createChar(4, block_80);
  lcd.createChar(5, block_full);
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

void display_settings(uint8_t sel_index, uint8_t cursor_offset) {
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
  //lcd.blink();
  lcd.setCursor(strlen(settings_lut[sel_index]) + cursor_offset, (sel_index % 3) + 1);
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