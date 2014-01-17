#include "globals.h"

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
