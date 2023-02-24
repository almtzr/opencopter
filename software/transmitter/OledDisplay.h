#ifndef __OLEDDISPLAY_H__
#define __OLEDDISPLAY_H__

#include "RCChannel.h"
#include "ButtonState.h"
#include "BatteryLife.h"

void saveEEPROM(bool reverseUpdate, bool reverseDefault, int address);
void draw();

class OledDisplay {

      public:
            OledDisplay ();
            void init ();        
            void screenChannel ();
            void screenChannelSettings ();
            void screenArmed ();
            void screenIntro ();
            void displayBattery ();
            void blinkArmed ();
            void drawOledRC ();
            void oledGPS ();
            void oledRCRevTH ();
            void oledRCRevYA ();
            void oledRCRevRO ();
            void oledRCRevPI ();
            void oledRCRevQUIT ();
            void screen (ButtonState * btnState, RCChannel * rc, BatteryLife * batLife);

      private:
            int m_levelTh, m_levelYa, m_levelRo, m_levelPi;
            bool m_reverseRightTH, m_reverseRightYA, m_reverseRightRO, m_reverseRightPI; 
            bool m_reverseRightDefaultTH, m_reverseRightDefaultYA, m_reverseRightDefaultRO, m_reverseRightDefaultPI;
            bool m_isBtnRightOn, m_isBtnLeftOn, m_isBtnCenterOn, m_isBtnArmedOn;
            int m_ch1, m_ch2, m_ch3, m_ch4;
            bool m_isPressBtnRight, m_isPressBtnLeft, m_isPressBtnCenter, m_isPressBtnArmed;
            int m_currentScreen;
            int m_revActive, m_intervalIntro, m_intervalArmed;
            unsigned long m_previousMillis;
            bool m_selectYES, m_displayARMED;
            bool m_revActiveTH, m_revActiveYA, m_revActiveRO, m_revActivePI;
            double m_batteryLife;
};


#endif