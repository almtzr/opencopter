#ifndef __RFCOMMUNICATION_H__
#define __RFCOMMUNICATION_H__

#include "RCChannel.h"
#include "ButtonState.h"
#include <Arduino.h>
#include "RF24.h"

struct channelData {
  byte ch1;  
  byte ch2;
  byte ch3;
  byte ch4;
};

class RFCommunication {
  public:
    RFCommunication ();
    void encodeData (RF24 * radio, ButtonState * btnState, RCChannel * rc);
    void decodeData ();

    void cmdArmed ();
    void cmdDisarmed ();
  private:
    channelData m_chData;
    bool m_armed;

};



#endif