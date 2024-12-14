#include <HX711.h>

#include "HX711.h"

const int DT_PIN = D5;
const int SCK_PIN = D4;
const int sample_weight = 100;  

HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale();  
  scale.tare();
  Serial.println("Nothing on it.");
  Serial.println(scale.get_units(10));
  Serial.println("Please put sapmple object on it..."); 
  
}

void loop() {
  float current_weight=scale.get_units(10);  
  float scale_factor=(current_weight/sample_weight);
  Serial.print("Scale number:  ");
  Serial.println(scale_factor,0);  
  // delay(1000);
}