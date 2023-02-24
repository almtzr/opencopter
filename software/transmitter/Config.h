#ifndef CONFIG_H
#define CONFIG_H
#include "LibCommon.h"
#include <EEPROM.h>

#define RELEASE "v.4.0" 
//#include <string>

// OLED DISPLAY GLOBAL CONSTANT
#define TITLE_RC_REVERSE_SCREEN "Save Channel Settings" 
#define TITLE_GPS_SCREEN "GPS Data" 
#define RC_REVERSE_TH "Throttle" 
#define RC_REVERSE_YA "Yaw" 
#define RC_REVERSE_RO "Roll" 
#define RC_REVERSE_PI "Pitch" 
#define RC_REVERSE_QUIT "Exit" 
#define DEFAULT_SCREEN 0
#define EEPROM_ADDR_TH 11
#define EEPROM_ADDR_YA 22
#define EEPROM_ADDR_RO 33
#define EEPROM_ADDR_PI 44
#define EEPROM_ADDR_WRITE_NB 55
#define TITLE_DISARMED "DISARMED" 
#define TITLE_ARMED "ARMED" 
#define TITLE_DLE "OPEN COPTER" 

#define LEVELWIDTH 13
#define SCREEN_SIZE_X 128
#define SCREEN_SIZE_Y 64
#define CHANNEL_WIDTH 12
#define CHANNEL_LENGHT 100
// PIN BUTTON
#define BTNLEFT   5
#define BTNCENTER 6
#define BTNRIGHT  4
#define BTNARMED  7

// RC CHANNEL GLOBAL CONSTANT
#define ANALOGPIN_MAX 1024
#define DIGITALPIN_MAX 255
#define PIN_MIN 0

#endif // CONFIGURATION_H