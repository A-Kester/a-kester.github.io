#include "Adafruit_VL53L0X.h"
#include <math.h>

#define MAX_SIZE 10
#define BOWL_RAD_MM 15

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int tof_data[MAX_SIZE] = {0}; // initialize measurements to 0
int top = -1; // empty stack
double step = (double)BOWL_RAD_MM / MAX_SIZE;
double bowl_area = 0;
double dough_area = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) {
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 

  // get empty bowl measurements
  Serial.println("Place empty bowl for intialization measurements.");
  delay(1000); // change to wait for button push once set up
  while (top <= MAX_SIZE - 1) {
    getMeasurements();
  }
  bowl_area = riemannSum();
 //Serial.print("Step: ");
  //Serial.println(step);
}

void loop() {
  // put your main code here, to run repeatedly:
  getMeasurements();

  //Serial.print("Top is: ");
  //Serial.println(top);
  if(top == MAX_SIZE - 1) {     // calculate volume when measurements are done
    dough_area = riemannSum();

    // Serial.print("The bowl area is: ");
    // Serial.println(bowl_area);
    // Serial.print("The dough area is: ");
    // Serial.println(dough_area);

    double breadVolume = getVolume();
    Serial.print("The current volume of the bread is ");
    Serial.print(breadVolume);
    Serial.println(" mm^3");
    delay(10000);
  }

 delay(1000); // change to wait for button push or motor movement, etc. 
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
  // Serial.print("Sum no pi: ");
  // Serial.println(volume);
  // Serial.print("Step: ");
  // Serial.println(step);
  volume *= 2 * M_PI * step;
  // Serial.print("Summ: ");
  // Serial.println(volume);
  volume = volume * (bowl_area - dough_area); // multiply area btwn. the curves
  return volume;
}
