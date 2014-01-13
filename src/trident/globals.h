#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>

static size_t const UINT32_MAX = 4294967295;

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

#endif  /* _GLOBALS_H_ */
