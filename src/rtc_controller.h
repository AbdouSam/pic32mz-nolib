#ifndef _RTCCONTROLLER_H
#define _RTCCONTROLLER_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

/* Standard libraries. */

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* Config includes. */

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Public Types
 *****************************************************************************/

typedef enum
{
  RTC_SUN = 1,
  RTC_MON = 2,
  RTC_TUE = 3,
  RTC_WED = 4,
  RTC_THU = 5,
  RTC_FRI = 6,
  RTC_SAT = 7
}rtc_weekday;

typedef enum
{
  RTC_JAN = 1,
  RTC_FEB = 2,
  RTC_MAR = 3,
  RTC_APR = 4,
  RTC_MAY = 5,
  RTC_JUN = 6,
  RTC_JUL = 7,
  RTC_AUG = 8,
  RTC_SEP = 9,
  RTC_OCT = 10,
  RTC_NOV = 11,
  RTC_DEC = 12
}rtc_month;

typedef enum
{
  RTC_CMP_ERROR,
  RTC_CMP_EARLIER,
  RTC_CMP_SAME,
  RTC_CMP_LATER
}rtc_cmp_t;

typedef enum
{
  RTC_CMP_NO_SECONDS,
  RTC_CMP_ALL
}rtc_cmp_type_t;

typedef struct
{
  int weekday;
  int day;
  int month;
  int year;
}rtc_date;

typedef struct
{
  int hours;
  int minutes;
  int seconds;
}rtc_time;

typedef struct rtc_clock
{
  rtc_date date;
  rtc_time time;
}rtc_clock;

int rtc_controller_getclock(rtc_clock *clock);

#endif /* _RTCCONTROLLER_H */
