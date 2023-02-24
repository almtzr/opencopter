#include "ButtonState.h"
#include "Config.h"

ButtonState::ButtonState() {
    pinMode(BTNLEFT, INPUT);
    pinMode(BTNCENTER, INPUT);
    pinMode(BTNRIGHT, INPUT);
    pinMode(BTNARMED, INPUT);
    m_isBtnRightOn = false;
    m_isBtnLeftOn = false;
    m_isBtnCenterOn = false;
    m_isBtnArmedOn = false; 
}

bool ButtonState::getBtnRightState() { return m_isBtnRightOn; }
bool ButtonState::getBtnLeftState() { return m_isBtnLeftOn; }
bool ButtonState::getBtnCenterState() { return m_isBtnCenterOn; }
bool ButtonState::getBtnArmedState() { return m_isBtnArmedOn; }
void ButtonState::updateBtnState () { 
  m_isBtnRightOn = digitalRead(BTNRIGHT) == HIGH; 
  m_isBtnLeftOn = digitalRead(BTNLEFT) == HIGH; 
  m_isBtnCenterOn = digitalRead(BTNCENTER) == HIGH; 
  m_isBtnArmedOn = digitalRead(BTNARMED) == HIGH; 
}