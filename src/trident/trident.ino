#include "BMA250.h"

#include <Wire.h>  //Needed for BMA250.h

static BMA250 gs_bma;
void setup()
{
  Serial.begin(9600);
  gs_bma.setup();
}

void loop()
{
  gs_bma.update();

  // Print out the accelerometer data
  Serial.print("x: ");
  Serial.print(gs_bma.accelX());
  Serial.print(", y: ");
  Serial.print(gs_bma.accelY());
  Serial.print(", z:");
  Serial.print(gs_bma.accelZ());
  Serial.print(",  t: ");   
  Serial.print(gs_bma.temperature());
  Serial.println("degC");    
  
  delay(100);
}

