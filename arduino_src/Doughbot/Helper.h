#define NUM_OPTIONS 8
#define UP 1
#define DOWN 0
#define ASCII_INT_OFFSET 0x30

uint8_t settings = 0b10101110; 
uint8_t* time_array[] = {0,0,0,0};

uint8_t ascii_to_int(char c);
void int_setting_change(uint8_t dir, char* str, uint8_t index);
void print_uint8_bin(uint8_t* u, char* str);

uint8_t ascii_to_int(char c) {
  return (uint8_t) ((uint8_t) c - ASCII_INT_OFFSET);
};

void int_setting_change(uint8_t dir, char* str, uint8_t index) {
  char* c = str + index;
  *c += (dir == UP) ? 1 : -1;
  // Handle Wrap arounds
  if (*c == '/') *c = '9';
  if (*c == ':') *c = '0';
}

void print_uint8_bin(uint8_t* u, char* str) {
  for (uint8_t i = 7; i >= 0; i-- ) {
    //str[7-i] = ((u & (1<<i)) >> i) == 1 ? '1' : '0';
  }
  str[8] = '/0';
}