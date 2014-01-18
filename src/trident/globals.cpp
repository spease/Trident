#include "globals.h"

#ifdef TRIDENT_ACL
  uint32_t const TRIDENT_ACL_POST_PERIOD_MS=1000;
#endif
#ifdef TRIDENT_GPS
  uint32_t const TRIDENT_GPS_POST_PERIOD_MS=5000;
#endif
#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
  uint32_t const TRIDENT_TMPHMD_POST_PERIOD_MS=2000;
#endif
#ifdef TRIDENT_OUTPUT_NETWORK
    //PROGMEM char const * const TRIDENT_TEMPODB_API_KEY="048cee316ae74274bb4b2f7730eba236:25f9d7663f7147a2a5cf0b5635f0a28a";
  char const PROGMEM TRIDENT_TEMPODB_API_KEY_BASE64[]="MDQ4Y2VlMzE2YWU3NDI3NGJiNGIyZjc3MzBlYmEyMzY6MjVmOWQ3NjYzZjcxNDdhMmE1Y2YwYjU2MzVmMGEyOGE=";
  char const PROGMEM TRIDENT_TEMPODB_API_SERVER[]="api.tempo-db.com";
  //char const PROGMEM TRIDENT_TEMPODB_API_SERVER[]="10.15.1.186";
  char const PROGMEM TRIDENT_TEMPODB_API_URI[] = "/v1/series/key/%s/data/";
#endif

uint32_t timeSince_ms(uint32_t const i_time_ms)
{
  uint32_t const timeCurrent_ms=millis();
  if(timeCurrent_ms >= i_time_ms)
  {
    return (timeCurrent_ms-i_time_ms);
  }
  else
  {
    return (UINT32_MAX-i_time_ms)+timeCurrent_ms;
  }
}
