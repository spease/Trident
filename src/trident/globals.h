#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>

/********************----- Configuration -----********************/
#define TRIDENT_ACL
#ifdef TRIDENT_ACL
  PROGMEM static uint32_t const TRIDENT_ACL_POST_PERIOD_MS=1000;
#endif
#define TRIDENT_GPS
#ifdef TRIDENT_GPS
  PROGMEM static uint32_t const TRIDENT_GPS_POST_PERIOD_MS=1000;
#endif
#define TRIDENT_HMD
#define TRIDENT_TMP
#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
  PROGMEM static uint32_t const TRIDENT_TMPHMD_POST_PERIOD_MS=1000;
#endif
#define TRIDENT_OUTPUT_NETWORK
#ifdef TRIDENT_OUTPUT_NETWORK
  //PROGMEM char const * const TRIDENT_TEMPODB_API_KEY="048cee316ae74274bb4b2f7730eba236:25f9d7663f7147a2a5cf0b5635f0a28a";
  char PROGMEM const TRIDENT_TEMPODB_API_KEY_BASE64[]="MDQ4Y2VlMzE2YWU3NDI3NGJiNGIyZjc3MzBlYmEyMzY6MjVmOWQ3NjYzZjcxNDdhMmE1Y2YwYjU2\r\nMzVmMGEyOGE=";
  //PROGMEM char const * const TRIDENT_TEMPODB_API_SERVER="api.tempo-db.com";
  char const TRIDENT_TEMPODB_API_SERVER[]="10.0.0.4";
  //prog_char const TRIDENT_TEMPODB_API_URI[] = "/v1/series/key/%s/data/";
#endif
//#define TRIDENT_OUTPUT_SERIAL
//#define TRIDENT_LOG_SERIAL
/**************************************************/
#define TEMPODB_SERIES_KEY(a,b) char PROGMEM const series_##a[] = "/v1/series/key/device:1." b "/data/";
#define TEMPODB_SERIES_URI(a) reinterpret_cast<__FlashStringHelper const *>(series_##a)
/********************----- Dependencies -----********************/
#ifdef TRIDENT_HMD
#define TRIDENT_TMP
#endif
/**************************************************/

/********************----- Error Handling -----********************/
#ifdef TRIDENT_LOG_SERIAL
#define TRIDENT_ERROR(a) Serial.println(/*String(__FILE__) + ":" + String(__func__) + "()-" + */a)
#define TRIDENT_INFO(a) Serial.println(/*String(__FILE__) + ":" + String(__func__) + "()-" + */a)
#else
#define TRIDENT_ERROR(a)
#define TRIDENT_INFO(a)
#endif
/**************************************************/

/********************----- Posting -----********************/
#ifdef TRIDENT_OUTPUT_SERIAL
#define TRIDENT_POST(a) Serial.println(String("POST: ") + (a))
#else
#define TRIDENT_POST(a)
#endif
/**************************************************/

/********************----- Handy integers -----********************/
PROGMEM static size_t const UINT32_MAX = 4294967295;
/**************************************************/

/********************----- Utility Functions -----********************/
uint32_t timeSince_ms(uint32_t const i_time_ms);
/**************************************************/

#endif  /* _GLOBALS_H_ */
