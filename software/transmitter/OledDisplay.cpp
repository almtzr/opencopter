#include "U8glib.h"
#include "OledDisplay.h"
#include "Config.h"

#include <string.h>
using namespace std;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

OledDisplay::OledDisplay() {
    m_reverseRightTH = EEPROM.read( EEPROM_ADDR_TH ) == 1; // TH : 0 = Left, 1 = Right
    m_reverseRightYA = EEPROM.read( EEPROM_ADDR_YA ) == 1; // YA
    m_reverseRightRO = EEPROM.read( EEPROM_ADDR_RO ) == 1; // RO
    m_reverseRightPI = EEPROM.read( EEPROM_ADDR_PI ) == 1; // PI
    m_reverseRightDefaultTH = m_reverseRightTH;
    m_reverseRightDefaultYA = m_reverseRightYA;
    m_reverseRightDefaultRO = m_reverseRightRO;
    m_reverseRightDefaultPI = m_reverseRightPI;
	m_levelTh = SCREEN_SIZE_Y;
	m_levelYa = SCREEN_SIZE_Y;
	m_levelRo = SCREEN_SIZE_Y;
	m_levelPi = SCREEN_SIZE_Y;
    m_intervalIntro = 5000;
    m_intervalArmed = 500;

    m_previousMillis = 0;
    m_revActive = 1; 
    m_displayARMED = false;
    m_revActiveTH = false;
    m_revActiveYA = false;
    m_revActiveRO = false;
    m_revActivePI = false;
    m_selectYES   = false;
    m_isBtnRightOn = false; 
    m_isBtnLeftOn = false; 
    m_isBtnCenterOn = false;
    m_currentScreen = DEFAULT_SCREEN;

    init();
}

void OledDisplay::init() {        
    u8g.setFont(u8g_font_6x12r); // on utilise une fonte choisie parmis le tableau de référence : https://github.com/olikraus/u8glib/wiki/fontsize
    u8g.setColorIndex(1); // Instruction pourt mettre pixel à on.(affiche le pixel en blanc)
}

void OledDisplay::screenChannel() {  
    LibCommon lib;
    if (m_isPressBtnCenter) {
        if (not m_isBtnCenterOn) {
            m_currentScreen = 2;
            m_isBtnCenterOn = true;
        }
    } else {
        m_isBtnCenterOn = false;
    }
    u8g.firstPage();
    do {  
        displayBattery();
        u8g.setFont(u8g_font_7x14r);
        if (m_selectYES) {
            blinkArmed();
        } else {
            u8g.drawStr(SCREEN_SIZE_X/5, 10, TITLE_DISARMED);
        }
        u8g.setFont(u8g_font_6x12r);
        u8g.drawFrame(0, LEVELWIDTH, CHANNEL_LENGHT, CHANNEL_WIDTH);
        u8g.drawStr(CHANNEL_LENGHT+5, 24, "TH");
        u8g.drawFrame(0, LEVELWIDTH*2, CHANNEL_LENGHT, CHANNEL_WIDTH);
        u8g.drawStr(CHANNEL_LENGHT+5, 24+CHANNEL_WIDTH+1, "YA");
        u8g.drawFrame(0, LEVELWIDTH*3, CHANNEL_LENGHT, CHANNEL_WIDTH);
        u8g.drawStr(CHANNEL_LENGHT+5, 24+2*CHANNEL_WIDTH+2, "RO");
        u8g.drawFrame(0, LEVELWIDTH*4, CHANNEL_LENGHT, CHANNEL_WIDTH);
        u8g.drawStr(CHANNEL_LENGHT+5, 24+3*CHANNEL_WIDTH+3, "PI");
        drawOledRC(); // on va a la fonction draw
    } while( u8g.nextPage() ); // on marque ici la fin de la procédure d'affichage

   m_levelTh = lib.map(m_ch3, 0, DIGITALPIN_MAX, 0, CHANNEL_LENGHT-4);
   m_levelYa = lib.map(m_ch4, 0, DIGITALPIN_MAX, CHANNEL_LENGHT-4, 0);
   m_levelRo = lib.map(m_ch1, 0, DIGITALPIN_MAX, CHANNEL_LENGHT-4, 0);
   m_levelPi = lib.map(m_ch2, 0, DIGITALPIN_MAX, CHANNEL_LENGHT-4, 0);
}

void OledDisplay::drawOledRC() {
    u8g.drawBox(2, LEVELWIDTH+2, m_levelTh, 8);
    u8g.drawBox(2, LEVELWIDTH*2+2, m_levelYa, 8);
    u8g.drawBox(2, LEVELWIDTH*3+2, m_levelRo, 8);
    u8g.drawBox(2, LEVELWIDTH*4+2, m_levelPi, 8);
}

void OledDisplay::oledGPS() {  
    u8g.firstPage();
    do {  
        u8g.drawStr(0, SCREEN_SIZE_Y, "<");
        u8g.drawStr(40, SCREEN_SIZE_Y, TITLE_GPS_SCREEN);
        u8g.drawStr(121, SCREEN_SIZE_Y, ">");
    } while( u8g.nextPage() );
}

void OledDisplay::displayBattery() {  
        u8g.drawFrame(101, 0, 22, 8);
        u8g.drawBox(102, 0, 4, 8);  //0 - 25%
        u8g.drawBox(107, 0, 4, 8);  //26 - 50%
        u8g.drawBox(112, 0, 4, 8);  //51 - 75%
        //u8g.drawBox(117, 0, 4, 8);  //76 - 100%
        u8g.drawBox(123, 1, 2, 6);

        //u8g.drawBox(102, 2, m_batteryLife, 6);
        //u8g.drawBox(X, Y, L, H);
}

void OledDisplay::blinkArmed() {  
 
    unsigned long currentMillis = millis();
    if (currentMillis - m_previousMillis >= m_intervalArmed) {
        m_previousMillis = currentMillis;
        if (m_displayARMED == true) {
            m_displayARMED = false;
        } else {
            m_displayARMED = true;
        }
    }
    if (m_displayARMED) {
        u8g.drawStr(SCREEN_SIZE_X/3.5, 10, TITLE_ARMED);
    }        
        
}


void OledDisplay::screenIntro() {  
    u8g.firstPage();

    do {  
        unsigned long currentMillis = millis();
        if (currentMillis - m_previousMillis >= m_intervalIntro) {
            m_previousMillis = currentMillis;
            m_currentScreen = 1;
            if (m_displayARMED == true) {
                m_displayARMED = false;
            } else {
                m_displayARMED = true;
            }
        }

            u8g.drawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y);
            u8g.setColorIndex(0);
            /*
            if (m_displayARMED) {
                u8g.setFont(u8g_font_7x14);
                u8g.drawStr(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/4, ".");
                u8g.drawStr(SCREEN_SIZE_X/3, SCREEN_SIZE_Y/4, ".");
                u8g.drawStr(SCREEN_SIZE_X/4, SCREEN_SIZE_Y/4, ".");
                u8g.drawStr(SCREEN_SIZE_X/5, SCREEN_SIZE_Y/4, ".");
                u8g.drawStr(SCREEN_SIZE_X/6, SCREEN_SIZE_Y/4, ".");
                u8g.drawStr(SCREEN_SIZE_X/7, SCREEN_SIZE_Y/4, ".");
                
            }
            */
            u8g.setFont(u8g_font_8x13B);
            u8g.drawStr(SCREEN_SIZE_X/6, SCREEN_SIZE_Y/2, TITLE_DLE);
            u8g.setFont(u8g_font_6x13B);
            u8g.drawStr(SCREEN_SIZE_X/1.6, SCREEN_SIZE_Y/1.4, RELEASE);
            u8g.setColorIndex(1);
                
        
    } while( u8g.nextPage() );
}


void OledDisplay::screenArmed() {  
    u8g.firstPage();

    do {  
        displayBattery();
        u8g.setFont(u8g_font_10x20r);
        u8g.drawStr(SCREEN_SIZE_X/3, SCREEN_SIZE_Y/2, TITLE_ARMED);
        u8g.setFont(u8g_font_6x12r);
        if (m_isPressBtnRight) { m_selectYES = false; }
        if (m_isPressBtnLeft) { m_selectYES = true; }
        if (m_selectYES) {
            u8g.drawBox(3, SCREEN_SIZE_Y - 12, SCREEN_SIZE_X/5, 16);
            u8g.setColorIndex(0);
            u8g.drawStr(5, SCREEN_SIZE_Y - 2, "YES");
            u8g.setColorIndex(1);
            u8g.drawStr(112, SCREEN_SIZE_Y - 2, "NO");
        } else {        
            u8g.drawBox(108, SCREEN_SIZE_Y - 12, SCREEN_SIZE_X/5, 16);
            u8g.setColorIndex(0);
            u8g.drawStr(112, SCREEN_SIZE_Y - 2, "NO");
            u8g.setColorIndex(1);
            u8g.drawStr(5, SCREEN_SIZE_Y - 2, "YES");
        }
        if (m_isPressBtnCenter) {
            if (not m_isBtnCenterOn) {
                m_currentScreen = 1;
                m_isBtnCenterOn = true;
            }   
        } else {
            m_isBtnCenterOn = false;
        }         
    } while( u8g.nextPage() );
}

void OledDisplay::screenChannelSettings() { 
    u8g.firstPage();
    do {  
        oledRCRevTH();
        oledRCRevYA();
        oledRCRevRO();
        oledRCRevPI();
        oledRCRevQUIT();
    if (m_isPressBtnCenter) {
        if (not m_isBtnCenterOn) {
            if (m_revActive == 1) {
                if (m_revActiveTH) { m_revActiveTH = false; } else { m_revActiveTH = true; }
            } else if (m_revActive == 2) {
                if (m_revActiveYA) { m_revActiveYA = false; } else { m_revActiveYA = true; }           
            } else if (m_revActive == 3) {
                if (m_revActiveRO) { m_revActiveRO = false; } else { m_revActiveRO = true; } 
            } else if (m_revActive == 4) {
                if (m_revActivePI) { m_revActivePI = false; } else { m_revActivePI = true; } 
            } else if (m_revActive == 5) {
                saveEEPROM(m_reverseRightTH, m_reverseRightDefaultTH, EEPROM_ADDR_TH);
                saveEEPROM(m_reverseRightYA, m_reverseRightDefaultYA, EEPROM_ADDR_YA);
                saveEEPROM(m_reverseRightRO, m_reverseRightDefaultRO, EEPROM_ADDR_RO);
                saveEEPROM(m_reverseRightPI, m_reverseRightDefaultPI, EEPROM_ADDR_PI);
                m_currentScreen = 1;
                m_revActive = 1; 
            }
            m_isBtnCenterOn = true;
        }   
    } else {
        m_isBtnCenterOn = false;
    } 
    if (not m_revActiveTH and not m_revActiveYA and not m_revActiveRO and not m_revActivePI) {

        if (m_isPressBtnRight) {
            if (not m_isBtnRightOn) {
                if (m_revActive < 5) {
                    m_revActive++;
                }
                m_isBtnRightOn = true;
            }   
        } else {
            m_isBtnRightOn = false;
        }
        if (m_isPressBtnLeft) {
            if (not m_isBtnLeftOn) {
                if (m_revActive > 1) {
                    m_revActive--;
                }
                m_isBtnLeftOn = true;
            }   
        } else {
            m_isBtnLeftOn = false;
        }
    }
          } while( u8g.nextPage() );

}

void OledDisplay::oledRCRevQUIT() { 
    if (m_revActive == 5) {
        u8g.drawBox(0, SCREEN_SIZE_Y - 12, SCREEN_SIZE_X, 12);
        u8g.setColorIndex(0);
        u8g.drawStr(2, SCREEN_SIZE_Y - 3, TITLE_RC_REVERSE_SCREEN);
        u8g.setColorIndex(1);
    } else {
        u8g.drawStr(2, SCREEN_SIZE_Y - 3, TITLE_RC_REVERSE_SCREEN);
    } 
}

void OledDisplay::oledRCRevTH() {  
    u8g.drawFrame(0, 0, SCREEN_SIZE_X/2, 12);
    if (m_revActiveTH) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 3, 3, 4, 8);
        if (m_isPressBtnRight) {
            m_reverseRightTH = true; 
        } 
        if (m_isPressBtnLeft) {
            m_reverseRightTH = false; 
        } 
    } 
    if (m_revActive == 1) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 10, 0, SCREEN_SIZE_X/2 - 14, 12);
        u8g.setColorIndex(0);
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, 10, RC_REVERSE_TH);
        u8g.setColorIndex(1);
    } else {
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, 10, RC_REVERSE_TH);
    }
    if (m_reverseRightTH) {
        u8g.drawBox(SCREEN_SIZE_X/4, 2 , SCREEN_SIZE_X/4 - 2, 8);  
    } else {
        u8g.drawBox(2, 2 , SCREEN_SIZE_X/4 - 2, 8);  
    }
}

void OledDisplay::oledRCRevYA() {  
    u8g.drawFrame(0, LEVELWIDTH, SCREEN_SIZE_X/2, 12);
    if (m_revActiveYA) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 3, LEVELWIDTH + 3, 4, 8);
        if (m_isPressBtnRight) {
            m_reverseRightYA = true; 
        } 
        if (m_isPressBtnLeft) {
            m_reverseRightYA = false; 
        } 
    }
    if (m_revActive == 2) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 10, LEVELWIDTH, SCREEN_SIZE_X/2 - 14,12);
        u8g.setColorIndex(0);
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH + 10, RC_REVERSE_YA);
        u8g.setColorIndex(1);
    } else {
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH + 10, RC_REVERSE_YA);
    } 
    if (m_reverseRightYA) {
        u8g.drawBox(SCREEN_SIZE_X/4, LEVELWIDTH + 2 , SCREEN_SIZE_X/4 - 2, 8);  
    } else {
        u8g.drawBox(2, LEVELWIDTH + 2 , SCREEN_SIZE_X/4 - 2, 8); 
    } 
    
}

void OledDisplay::oledRCRevRO() {  
    u8g.drawFrame(0, LEVELWIDTH*2, SCREEN_SIZE_X/2, 12);
    if (m_revActiveRO) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 3, LEVELWIDTH*2 + 3, 4, 8);
        if (m_isPressBtnRight) {
            m_reverseRightRO = true; 
        } 
        if (m_isPressBtnLeft) {
            m_reverseRightRO = false; 
        } 
    }
    if (m_revActive == 3) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 10, LEVELWIDTH*2, SCREEN_SIZE_X/2 - 14,12);
        u8g.setColorIndex(0);
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH*2 + 10, RC_REVERSE_RO);
        u8g.setColorIndex(1);
    } else {
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH*2 + 10, RC_REVERSE_RO);
    }     
    if (m_reverseRightRO) {
        u8g.drawBox(SCREEN_SIZE_X/4, LEVELWIDTH*2 + 2 , SCREEN_SIZE_X/4 - 2, 8);
    } else {
        u8g.drawBox(2, LEVELWIDTH*2 + 2 , SCREEN_SIZE_X/4 - 2, 8);   
    } 
}

void OledDisplay::oledRCRevPI() {  
    u8g.drawFrame(0, LEVELWIDTH*3, SCREEN_SIZE_X/2, 12);
    if (m_revActivePI) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 3, LEVELWIDTH*3 + 3, 4, 8);
        if (m_isPressBtnRight) {
            m_reverseRightPI = true; 
        } 
        if (m_isPressBtnLeft) {
            m_reverseRightPI = false; 
        } 
    } 
    if (m_revActive == 4) {
        u8g.drawBox(SCREEN_SIZE_X/2 + 10, LEVELWIDTH*3, SCREEN_SIZE_X/2 - 14,12);
        u8g.setColorIndex(0);
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH*3 + 10, RC_REVERSE_PI);
        u8g.setColorIndex(1);
    } else {
        u8g.drawStr(SCREEN_SIZE_X/2 + 12, LEVELWIDTH*3 + 10, RC_REVERSE_PI);
    } 
    if (m_reverseRightPI) {
        u8g.drawBox(SCREEN_SIZE_X/4, LEVELWIDTH*3 + 2 , SCREEN_SIZE_X/4 - 2, 8);
    } else {
        u8g.drawBox(2, LEVELWIDTH*3 + 2 , SCREEN_SIZE_X/4 - 2, 8);   
    } 
}

void saveEEPROM(bool reverseUpdate, bool reverseDefault, int address) {
    if (reverseUpdate != reverseDefault) {
        EEPROM.write(EEPROM_ADDR_WRITE_NB, EEPROM.read(EEPROM_ADDR_WRITE_NB) + 1); //Control number of EEPROM write acces
        if (reverseUpdate) {
            EEPROM.write(address, 1);
        } else {
            EEPROM.write(address, 0);
        }
    }
}

void OledDisplay::screen (ButtonState * btnState, RCChannel * rc, BatteryLife * batLife) {  

    m_isPressBtnRight = btnState->getBtnRightState ();
    m_isPressBtnLeft = btnState->getBtnLeftState ();
    m_isPressBtnCenter = btnState->getBtnCenterState ();
    m_isPressBtnArmed = btnState->getBtnArmedState ();

    m_ch1 = rc->getCh1 ();
    m_ch2 = rc->getCh2 ();
    m_ch3 = rc->getCh3 ();
    m_ch4 = rc->getCh4 ();

    rc->setReverseRightTH (m_reverseRightTH);
    rc->setReverseRightYA (m_reverseRightYA);
    rc->setReverseRightRO (m_reverseRightRO);
    rc->setReverseRightPI (m_reverseRightPI);

    m_batteryLife = batLife->getBatteryLife ();

    if (m_isPressBtnArmed) {
        m_currentScreen = 3;
    }
    
    switch ( m_currentScreen )
    {
        
        case 0:
            screenIntro ();
            break;
        case 1:
            screenChannel ();
            break;
        case 2:
            screenChannelSettings ();
            break;
        case 3:
            screenArmed ();
            break;
        default:
            screenIntro ();
    } 
}