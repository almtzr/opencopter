#include "RCChannel.h"
#include "Config.h"

const int level = 0;

RCChannel::RCChannel() {
  m_reverseRightTH = EEPROM.read( EEPROM_ADDR_TH ) == 1; // TH : 0 = Left, 1 = Right
  m_reverseRightYA = EEPROM.read( EEPROM_ADDR_YA ) == 1; // YA
  m_reverseRightRO = EEPROM.read( EEPROM_ADDR_RO ) == 1; // RO
  m_reverseRightPI = EEPROM.read( EEPROM_ADDR_PI ) == 1; // PI

  m_ch1 = level;
  m_ch2 = level;
  m_ch3 = level;
  m_ch4 = level;

  /*sent_data.ch1 = 127;  // Roll
  sent_data.ch2 = 127;  // Ptch
  sent_data.ch3 = 255;  // Throttle
  sent_data.ch4 = 127;  // Yaw
  */
}

int RCChannel::getCh1() { return m_ch1; }
int RCChannel::getCh2() { return m_ch2; }
int RCChannel::getCh3() { return m_ch3; }
int RCChannel::getCh4() { return m_ch4; }

void RCChannel::setReverseRightTH(bool reverse) { m_reverseRightTH = reverse; }
void RCChannel::setReverseRightRO(bool reverse) { m_reverseRightRO = reverse; }
void RCChannel::setReverseRightYA(bool reverse) { m_reverseRightYA = reverse; }
void RCChannel::setReverseRightPI(bool reverse) { m_reverseRightPI = reverse; }

void RCChannel::updateAnalogChannel(long analogA2, long analogA3, long analogA0, long analogA1) { 
  //Normal:    data.ch1 = map( analogRead(A0), 0, 1024, 0, 255 );
  //Reversed:  data.ch1 = map( analogRead(A0), 0, 1024, 255, 0 );
  LibCommon lib;
  if (m_reverseRightRO) {
    m_ch1 = lib.map(analogA2, PIN_MIN, ANALOGPIN_MAX, DIGITALPIN_MAX, PIN_MIN); // Normal
  } else {
     m_ch1 = lib.map(analogA2, PIN_MIN, ANALOGPIN_MAX, PIN_MIN, DIGITALPIN_MAX); // Reversed
  } 

  if (m_reverseRightPI) {
    m_ch2 = lib.map(analogA1, PIN_MIN, ANALOGPIN_MAX, PIN_MIN, DIGITALPIN_MAX); // Normal
  } else {
     m_ch2 = lib.map(analogA1, PIN_MIN, ANALOGPIN_MAX, DIGITALPIN_MAX, PIN_MIN); // Reversed
  }

  if (m_reverseRightTH) {
    m_ch3 = lib.map(analogA0, PIN_MIN, ANALOGPIN_MAX, DIGITALPIN_MAX, PIN_MIN); // Normal
  } else {
    m_ch3 = lib.map(analogA0, PIN_MIN, ANALOGPIN_MAX, PIN_MIN, DIGITALPIN_MAX); // Reversed
  }

  if (m_reverseRightYA) {
    m_ch4 = lib.map(analogA3, PIN_MIN, ANALOGPIN_MAX, PIN_MIN, DIGITALPIN_MAX); // Normal
  } else {
    m_ch4 = lib.map(analogA3, PIN_MIN, ANALOGPIN_MAX, DIGITALPIN_MAX, PIN_MIN); // Reversed
  }
  //updateChannel();
}


void RCChannel::updateChannel() {
  m_ch1 = 127;  // Roll
  m_ch2 = 127;  // Ptch
  m_ch3 = 255;  // Throttle
  m_ch4 = 127;  // Yaw
}
