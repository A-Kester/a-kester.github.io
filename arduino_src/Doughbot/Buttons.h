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

void start_isr();
void up_isr();
void down_isr();
void swap_isr();
void buttons_init();

uint8_t has_been_pressed(uint8_t button);

uint8_t button_status = 0;

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
    case B_UP:
      if ((button_status & UP_MASK) > 0) {
      // Handle up button Pressed
        //Serial.println("Up");
        button_status &= ~UP_MASK; // clear the start bit
        return 1;
      }
      break;
    case B_DOWN:
      if ((button_status & DOWN_MASK) > 0) {
        Serial.println("Down");
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
    default:break;
  }
  return 0;
}

void buttons_init() {
   // Set Pins
  pinMode(B_SWAP, INPUT_PULLDOWN);
  pinMode(B_DOWN, INPUT_PULLDOWN);
  pinMode(B_UP, INPUT_PULLDOWN);
  pinMode(B_START, INPUT_PULLDOWN);
  
  // Attach Button Interupts
  attachInterrupt(digitalPinToInterrupt(B_SWAP), swap_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_DOWN), up_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_UP), down_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(B_START), start_isr, RISING);
}