#ifndef __BATTERYLIFE_H__
#define __BATTERYLIFE_H__

class BatteryLife {

    public:
        BatteryLife();
        void updateBatteryLife (double batteryLife);
        double getBatteryLife ();

    private:
        double m_batteryLife;
};

#endif