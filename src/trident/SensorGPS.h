#ifndef _SensorGPS_H_
#define _SensorGPS_H_

#include "globals.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

PROGMEM static int const SENSORGPS_BAUD = 9600;
PROGMEM static int const SENSORGPS_pinPower = A3;
PROGMEM static int const SENSORGPS_pinPowerStatus = A2;
PROGMEM static int const SENSORGPS_pinRX = A1;
PROGMEM static int const SENSORGPS_pinTX = A0;

class SensorGPS
{
public:
  SensorGPS()
  :m_link(SENSORGPS_pinRX, SENSORGPS_pinTX)
  {}
  
  TinyGPSPlus &data()
  {
    return m_tinyGPS;
  }
  
  void setup()
  {
    TRIDENT_INFO(F("GPS"));
    
    pinMode(SENSORGPS_pinPower, OUTPUT);
    pinMode(SENSORGPS_pinPowerStatus, INPUT);
    digitalWrite(SENSORGPS_pinPower, LOW);
    delay(5);
    if(digitalRead(SENSORGPS_pinPowerStatus) == LOW)
    {
      //Wake module
      digitalWrite(SENSORGPS_pinPower, HIGH);
      delay(5);
      digitalWrite(SENSORGPS_pinPower, LOW);
    }
    
    m_link.begin(SENSORGPS_BAUD);
  }
  
  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    while(m_link.available() > 0)
    {
      m_tinyGPS.encode(m_link.read());
      if(timeSince_ms(i_timeStart_ms) >= i_budget_ms)
      {
        return;
      }
    }
  }
private:
  TinyGPSPlus m_tinyGPS;
  SoftwareSerial m_link;
};

#endif  /* _SensorGPS_H_ */
