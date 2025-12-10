#include "Adafruit_VL53L0X.h"
#include <math.h>

#define MAX_SIZE 10
#define BOWL_RAD_MM 15

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int tof_data[MAX_SIZE] = {0}; // initialize measurements to 0
double step = (double)BOWL_RAD_MM / MAX_SIZE;
int top = -1; // empty stack
double bowl_area = 0;
double dough_area = 0;

void push(int val);
int get_mil();
void getMeasurements();

uint8_t TOF_init() {
  if (!lox.begin(15)) {
    Serial.println(F("Failed to boot VL53L0X"));
    return 0;
  }
  return 1;
}

void push(int val) {
  if (top == MAX_SIZE) {
    Serial.println("Stack Overflow");
  } else {
    top++;
    tof_data[top] = val;
  }
}

int get_mil() {
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    int mil = measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); 
    Serial.println(mil);
    return mil;
  } else {
    Serial.println(" out of range ");
    return -1;
  }
}

void getMeasurements() {
  
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout! 

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    int measurement = measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); 
    Serial.println(measurement);
    push(measurement);
  } else {
    Serial.println(" out of range ");
  }
  
  delay(100);
}

