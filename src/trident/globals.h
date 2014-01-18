#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>

/********************----- Configuration -----********************/
#define TRIDENT_ACL
#define TRIDENT_GPS
#define TRIDENT_HMD
#define TRIDENT_TMP
#define TRIDENT_OUTPUT_NETWORK
//#define TRIDENT_OUTPUT_SERIAL
//#define TRIDENT_LOG_SERIAL
/**************************************************/

/********************----- Dependencies -----********************/
#ifdef TRIDENT_HMD
#define TRIDENT_TMP
#endif
/**************************************************/

/********************----- Configuration Variables -----********************/
#ifdef TRIDENT_ACL
  extern uint32_t const TRIDENT_ACL_POST_PERIOD_MS;
#endif
#ifdef TRIDENT_GPS
  extern uint32_t const TRIDENT_GPS_POST_PERIOD_MS;
#endif
#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
  extern uint32_t const TRIDENT_TMPHMD_POST_PERIOD_MS;
#endif
#ifdef TRIDENT_OUTPUT_NETWORK
  extern char const PROGMEM TRIDENT_TEMPODB_API_KEY_BASE64[];
  extern char const PROGMEM TRIDENT_TEMPODB_API_SERVER[];
  extern char const PROGMEM TRIDENT_TEMPODB_API_URI[];
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
static size_t const UINT32_MAX = 4294967295;
/**************************************************/

/********************----- Utility Functions -----********************/
uint32_t timeSince_ms(uint32_t const i_time_ms);
/**************************************************/

#endif  /* _GLOBALS_H_ */
