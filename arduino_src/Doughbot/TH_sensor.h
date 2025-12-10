// TEMP/HUMID Includes
#include <Adafruit_AHTX0.h>
// Screen Library:

Adafruit_AHTX0 aht;

struct temp_humid {
  float temp;
  float humid;
};

void init_temp_humid();
void get_temp_humid(struct temp_humid *);

void init_temp_humid(){
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    //while (1) delay(10);
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