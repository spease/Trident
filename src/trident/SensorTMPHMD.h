#ifndef _SENSOR_TMPHMD_H_
#define _SENSOR_TMPHMD_H_

#include "globals.h"

#include <Sensirion.h>

PROGMEM static int const SENSORGPS_PIN_DATA=5;
PROGMEM static int const SENSORGPS_PIN_CLOCK=6;

class SensorTMPHMD
{
public:
  SensorTMPHMD()
  :m_humidity(0),m_temperature(0),m_sensor(SENSORGPS_PIN_DATA, SENSORGPS_PIN_CLOCK),m_status(REQUESTED_NOTHING)
  {
  }
  
  float humidity() const
  {
    return m_humidity;
  }
  
  float temperature() const
  {
    return m_temperature;
  }
  
  void setup()
  {
    TRIDENT_INFO("TMPHMD");
  }
  
  void update()
  {
    if(m_sensor.measRdy() == S_Meas_Rdy && m_status != REQUESTED_NOTHING)
    {
      if(m_status == REQUESTED_TEMPERATURE)
      {
        m_temperature = m_sensor.calcTemp(m_data);
        m_sensor.meas(HUMI, &m_data, false);
        m_status = REQUESTED_HUMIDITY;
      }
      else
      {
        if(m_status == REQUESTED_HUMIDITY)
        {
          m_humidity = m_sensor.calcHumi(m_data, m_temperature);
        }
        m_sensor.meas(TEMP, &m_data, false);
        m_status = REQUESTED_TEMPERATURE;
      }
    }
    else
    {
      /**** Request temperature *****/
      //Always do first, since humidity depends on it.
      m_sensor.meas(TEMP, &m_data, false);
      m_status = REQUESTED_TEMPERATURE;
    }
  }
private:
  enum State
  {
    REQUESTED_NOTHING=0,
    REQUESTED_HUMIDITY=1,
    REQUESTED_TEMPERATURE=2,
  };
  Sensirion m_sensor;
  State m_status;
  uint16_t m_data;
  float m_humidity;
  float m_temperature;
};

#endif  /* _SENSOR_TMPHMD_H_ */
