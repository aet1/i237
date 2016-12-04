#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define STUD_NAME "Aet Udusaar"
#define GET_MONTH_NAME "Enter Month name first letter >"
#define PRG_VER "Version: %s built on: %s %s"
#define LIBC_VER "avr-libc version: %s avr-gcc version: %s"
const char month1[]  PROGMEM = "January";
const char month2[] PROGMEM = "February";
const char month3[]    PROGMEM = "March";
const char month4[]    PROGMEM = "April";
const char month5[]      PROGMEM = "May";
const char month6[]     PROGMEM = "June";

PGM_P const months[] PROGMEM = {month1,month2,month3,month4,month5,month6};
#endif /*_HMI_MSG_H_*/

