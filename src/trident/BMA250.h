#ifndef _BMA250_H_
#define _BMA250_H_
#include "globals.h"

#include <Wire.h>

/********************----- ENUM: Bandwidth -----********************/
enum Bandwidth
{
  BMA250_BANDWIDTH_7_81_HZ=0x08,  //7.81Hz (update time of 64ms)
};
/**************************************************/

/********************----- ENUM: Range -----********************/
enum Range
{
  BMA250_RANGE_2G = 0x03,
  BMA250_RANGE_4G = 0x05,
  BMA250_RANGE_8G = 0x08,
  BMA250_RANGE_16G = 0x0C
};
/**************************************************/

/********************----- BMA250 defaults -----********************/
static Bandwidth const BMA250_BANDWIDTH_DEFAULT = BMA250_BANDWIDTH_7_81_HZ;
static int const I2CADDR = 0x18;
static Range const BMA250_RANGE_DEFAULT = BMA250_RANGE_2G;
/**************************************************/

/********************----- CLASS: BMA250 -----********************/
template<Bandwidth t_bandwidth=BMA250_BANDWIDTH_DEFAULT, Range t_range=BMA250_RANGE_DEFAULT>
class BMA250
{
public:
  BMA250()
  :m_accelX(0),m_accelY(0),m_accelZ(0),m_temperature(0)
  {
  }

  uint16_t accelX() const
  {
    return m_accelX;
  }

  uint16_t accelY() const
  {
    return m_accelY;
  }

  uint16_t accelZ() const
  {
    return m_accelZ;
  }

  void setup()
  {
    TRIDENT_INFO("BMA-"+String(TRIDENT_ACL_POST_PERIOD_MS));
    Wire.begin();

    // Setup the range measurement setting
    Wire.beginTransmission(I2CADDR);
    Wire.write(0x0F); 
    Wire.write(t_range);
    Wire.endTransmission();
    
    // Setup the bandwidth
    Wire.beginTransmission(I2CADDR);
    Wire.write(0x10);
    Wire.write(t_bandwidth);
    Wire.endTransmission();
  }
  
  float temperature() const
  {
    return m_temperature;
  }

  void update()
  { 
    uint8_t readBuffer[8];
    // Read the 7 data bytes from the BMA250
    Wire.beginTransmission(I2CADDR);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.requestFrom(I2CADDR,7);
    
    for(int i = 0; i < 7;i++)
    {
      readBuffer[i] = Wire.read();
    }
    
    m_accelX = readBuffer[1] << 8;
    m_accelX |= readBuffer[0];
    m_accelX >>= 6;
    
    m_accelY = readBuffer[3] << 8;
    m_accelY |= readBuffer[2];
    m_accelY >>= 6;
    
    m_accelZ = readBuffer[5] << 8;
    m_accelZ |= readBuffer[4];
    m_accelZ >>= 6;

    m_temperature = (readBuffer[6] * 0.5f) + 24.0f;
  }
private:
  uint16_t m_accelX;
  uint16_t m_accelY;
  uint16_t m_accelZ;
  
  float m_temperature;
};
/**************************************************/

#endif /* _BMA250_H_ */
