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
void getMeasurements();
double riemannSum();
double getVolume();

uint8_t TOF_init() {
  if (!lox.begin()) {
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

double getVolume() {
  double volume = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    volume += i;
  }
  volume *= 2 * M_PI * step;
  volume = volume * (bowl_area - dough_area); // multiply area btwn. the curves
  return volume;
}