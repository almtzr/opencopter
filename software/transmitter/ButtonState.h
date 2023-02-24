#ifndef __BUTTONSTATE_H__
#define __BUTTONSTATE_H__

#include <Arduino.h>
#include "Config.h"


class ButtonState {
  public:
    ButtonState ();
    bool getBtnRightState ();
    bool getBtnLeftState ();
    bool getBtnCenterState ();
    bool getBtnArmedState ();
    void updateBtnState ();
  private:
    bool m_isBtnRightOn, m_isBtnLeftOn, m_isBtnCenterOn, m_isBtnArmedOn;

};



#endif