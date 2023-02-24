#include "BatteryLife.h"

BatteryLife::BatteryLife() {
   m_batteryLife = 0;
}

void BatteryLife::updateBatteryLife(double batteryLife) { 
   m_batteryLife = batteryLife;   
}

double BatteryLife::getBatteryLife() { 
   return m_batteryLife; 
}
