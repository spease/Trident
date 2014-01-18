#ifndef _TEMPODB_H_
#define _TEMPODB_H_

#include "globals.h"
#include "HTTPConnection.h"
#include "NetworkConnection.h"

/********************----- Helpful Macros -----********************/
#define TEMPODB_SERIES_KEY(a,b) char PROGMEM const series_##a[] = "/v1/series/key/device:1." b "/data/";
#define TEMPODB_SERIES_URI(a) reinterpret_cast<__FlashStringHelper const *>(series_##a)
/**************************************************/

/********************----- Available Series -----********************/
TEMPODB_SERIES_KEY(GPS_LATITUDE, "GPS.Latitude.");
TEMPODB_SERIES_KEY(GPS_LONGITUDE, "GPS.Longitude.");
TEMPODB_SERIES_KEY(GPS_SATELLITES, "GPS.Satellites.");
TEMPODB_SERIES_KEY(GPS_SPEED, "GPS.Speed.");
TEMPODB_SERIES_KEY(GPS_COURSE, "GPS.Course.");
TEMPODB_SERIES_KEY(GPS_ALTITUDE, "GPS.Altitude.");
TEMPODB_SERIES_KEY(ACL_X, "ACL.AccelerationX.");
TEMPODB_SERIES_KEY(ACL_Y, "ACL.AccelerationY.");
TEMPODB_SERIES_KEY(ACL_Z, "ACL.AccelerationZ.");
TEMPODB_SERIES_KEY(ACL_TEMPERATURE, "ACL.Temperature.");
TEMPODB_SERIES_KEY(TMP_TEMPERATURE, "TMP.Temperature.");
TEMPODB_SERIES_KEY(HMD_HUMIDITY, "HMD.Humidity.");
/**************************************************/

/********************----- Series Mappings -----********************/
__FlashStringHelper const * const TEMPODB_SERIES_URIS[] = {
  TEMPODB_SERIES_URI(GPS_LATITUDE),
  TEMPODB_SERIES_URI(GPS_LONGITUDE),
  TEMPODB_SERIES_URI(GPS_SATELLITES),
  TEMPODB_SERIES_URI(GPS_SPEED),
  TEMPODB_SERIES_URI(GPS_COURSE),
  TEMPODB_SERIES_URI(GPS_ALTITUDE),
  TEMPODB_SERIES_URI(ACL_X),
  TEMPODB_SERIES_URI(ACL_Y),
  TEMPODB_SERIES_URI(ACL_Z),
  TEMPODB_SERIES_URI(ACL_TEMPERATURE),
  TEMPODB_SERIES_URI(TMP_TEMPERATURE),
  TEMPODB_SERIES_URI(HMD_HUMIDITY),
};
namespace TempoDBSeries
{
  enum Enum
  {
    GPS_LATITUDE=0,
    GPS_LONGITUDE=1,
    GPS_SATELLITES=2,
    GPS_SPEED=3,
    GPS_COURSE=4,
    GPS_ALTITUDE=5,
    ACL_X=6,
    ACL_Y=7,
    ACL_Z=8,
    ACL_TEMPERATURE=9,
    TMP_TEMPERATURE=10,
    HMD_HUMIDITY=11
  };
}
/**************************************************/

class TempoDB
{
public:
  TempoDB()
:
    m_connection(reinterpret_cast<__FlashStringHelper const *>(TRIDENT_TEMPODB_API_SERVER), reinterpret_cast<__FlashStringHelper const *>(TRIDENT_TEMPODB_API_KEY_BASE64))
    {
    }

  void postSeriesUInt32(TempoDBSeries::Enum const i_series, uint32_t const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
  void postSeriesDouble(TempoDBSeries::Enum const i_series, double const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);

  void setup()
  {
    m_connection.setup();

    TRIDENT_INFO("TDB");
  }

  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    m_connection.update(i_timeStart_ms, i_budget_ms);
  }

private:
  HTTPConnection m_connection;
};

#endif /* _TEMPODB_H_ */

