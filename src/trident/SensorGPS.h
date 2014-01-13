#ifndef _SensorGPS_H_
#define _SensorGPS_H_

#include "globals.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class SensorGPS
{
public:
  SensorGPS()
  :m_link(msc_pinRX, msc_pinTX)
  {}
  
  TinyGPSPlus &data()
  {
    return m_tinyGPS;
  }
  
  void setup()
  {
    pinMode(msc_pinPower, OUTPUT);
    pinMode(msc_pinPowerStatus, INPUT);
    digitalWrite(msc_pinPower, LOW);
    delay(5);
    if(digitalRead(msc_pinPowerStatus) == LOW)
    {
      //Wake module
      digitalWrite(msc_pinPower, HIGH);
      delay(5);
      digitalWrite(msc_pinPower, LOW);
    }
    
    m_link.begin(msc_baud);
  }
  
  void update(uint32_t const i_budget_ms)
  {
    uint32_t timeStart = millis();
    
    while(m_link.available() > 0)
    {
      m_tinyGPS.encode(m_link.read());
      if(timeSince_ms(timeStart) >= i_budget_ms)
      {
        return;
      }
    }
  }
private:
  static int const msc_baud = 9600;
  static int const msc_pinPower = A3;
  static int const msc_pinPowerStatus = A2;
  static int const msc_pinRX = A1;
  static int const msc_pinTX = A0;
  
  TinyGPSPlus m_tinyGPS;
  SoftwareSerial m_link;
};

#endif  /* _SensorGPS_H_ */
