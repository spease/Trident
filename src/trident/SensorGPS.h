#ifndef _SensorGPS_H_
#define _SensorGPS_H_
#include "pins.h"
#include "globals.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

PROGMEM static int const SENSORGPS_BAUD = 9600;

class SensorGPS
{
public:
  SensorGPS()
  :m_link(GPS_SERIAL_RX, GPS_SERIAL_TX)
  {}
  
  TinyGPSPlus &data()
  {
    return m_tinyGPS;
  }
  
  void setup()
  {
    TRIDENT_INFO("GPS-"+String(TRIDENT_GPS_POST_PERIOD_MS));
    
    pinMode(GPS_POWER, OUTPUT);
    pinMode(GPS_POWERSTATUS, INPUT);
    digitalWrite(GPS_POWER, LOW);
    delay(5);
    if(digitalRead(GPS_POWERSTATUS) == LOW)
    {
      //Wake module
      digitalWrite(GPS_POWER, HIGH);
      delay(5);
      digitalWrite(GPS_POWER, LOW);
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
