#ifndef __RCCHANNEL_H__
#define __RCCHANNEL_H__

// The sizeof this struct should not exceed 32 bytes
/*
struct Data_to_be_sent {
  byte ch1;  
  byte ch2;
  byte ch3;
  byte ch4;
};
*/

class RCChannel {

    public:
        RCChannel();
        int getCh1();
        int getCh2();
        int getCh3();
        int getCh4();
        void setReverseRightTH(bool reverse);
        void setReverseRightYA(bool reverse);
        void setReverseRightRO(bool reverse);
        void setReverseRightPI(bool reverse);
        void updateAnalogChannel(long analogA2, long analogA3, long analogA0, long analogA1);
        void sendRCChannel();
        void updateChannel();

    private:
       int m_ch1, m_ch2, m_ch3, m_ch4;
       bool m_reverseRightTH, m_reverseRightYA, m_reverseRightRO, m_reverseRightPI; 

       //Data_to_be_sent sent_data;

};



#endif