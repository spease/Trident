#ifndef _BMA250_H_
#define _BMA250_H_
#include <Wire.h>

/********************----- CLASS: BMA250 -----********************/
class BMA250
{
public:
  /********************----- ENUM: Bandwidth -----********************/
  enum Bandwidth
  {
    BANDWIDTH_7_81_HZ=0x08,  //7.81Hz (update time of 64ms)
  };
  /**************************************************/

  /********************----- ENUM: Range -----********************/
  enum Range
  {
    RANGE_2G = 0x03,
    RANGE_4G = 0x05,
    RANGE_8G = 0x08,
    RANGE_16G = 0x0C
  };
  /**************************************************/

  BMA250(Bandwidth const i_bandwidth=BANDWIDTH_DEFAULT, Range const i_range=RANGE_DEFAULT)
  :m_accelX(0),m_accelY(0),m_accelZ(0),
  m_bandwidth(i_bandwidth),m_range(i_range),m_temperature(0)
  {
  }

  int accelX() const
  {
    return m_accelX;
  }

  int accelY() const
  {
    return m_accelY;
  }

  int accelZ() const
  {
    return m_accelZ;
  }

  void setup()
  {
    Wire.begin();

    // Setup the range measurement setting
    Wire.beginTransmission(BMA250::I2CADDR);
    Wire.write(0x0F); 
    Wire.write(m_range);
    Wire.endTransmission();
    
    // Setup the bandwidth
    Wire.beginTransmission(BMA250::I2CADDR);
    Wire.write(0x10);
    Wire.write(m_bandwidth);
    Wire.endTransmission();
  }
  
  int temperature() const
  {
    return m_temperature;
  }

  void update()
  { 
    uint8_t readBuffer[8];
    // Read the 7 data bytes from the BMA250
    Wire.beginTransmission(BMA250::I2CADDR);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.requestFrom(BMA250::I2CADDR,7);
    
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
  static Bandwidth const BANDWIDTH_DEFAULT = BANDWIDTH_7_81_HZ;
  static int const I2CADDR = 0x18;
  static Range const RANGE_DEFAULT = RANGE_2G;

  int m_accelX;
  int m_accelY;
  int m_accelZ;

  Bandwidth const m_bandwidth;
  Range const m_range;
  
  float m_temperature;
};
/**************************************************/

#endif /* _BMA250_H_ */
