#include "RFCommunication.h"
#include "Config.h"

//const uint64_t radio_pipe = 0xE8E8F0F0E1LL; //Remember that this code should be the same for the receiver
//RF24 myRadio (9, 10);

RFCommunication::RFCommunication() {
  /*myRadio.begin();
  myRadio.setAutoAck(false);
  myRadio.setDataRate(RF24_2MBPS); //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS

  // Transmitter
  myRadio.openWritingPipe(radio_pipe); 
  myRadio.setChannel(120); //2400 + 120 = 2520MHz 76 default
  // Receiver
  //radio.openReadingPipe(1,radio_pipe);
  //radio.startListening();  //We start the radio comunication
*/
  m_armed = false;
  m_chData.ch1 = 127;  // Roll
  m_chData.ch2 = 127;  // Pitch
  m_chData.ch3 = 255;  // Throttle
  m_chData.ch4 = 127;  // Yaw
  
}

void RFCommunication::cmdArmed () { 
  m_armed = true;
}

void RFCommunication::cmdDisarmed () { 
  m_armed = false;
}

void RFCommunication::encodeData (RF24 * radio, ButtonState * btnState, RCChannel * rc) { 

 // if (btnState->getBtnArmedState()) {
 //   cmdArmed();
 // } else {
    m_chData.ch1 = rc->getCh1(); // Roll
    m_chData.ch2 = rc->getCh2(); // Pitch
    m_chData.ch3 = rc->getCh3(); // Throttle
    m_chData.ch4 = rc->getCh4(); // Yaw

   // unsigned long currentMillis = millis();
   // Serial.print ("currentMillis: ");
   // Serial.print (currentMillis);
    Serial.print ("Throttle: ");
    Serial.print (m_chData.ch3);
    Serial.print (" Yaw: ");
    Serial.print (m_chData.ch4);
    Serial.print (" Roll: ");
    Serial.print (m_chData.ch1);
    Serial.print (" Pitch: ");
    Serial.println (m_chData.ch2);

    radio->write(&m_chData, sizeof(channelData));
 // }
}

void RFCommunication::decodeData () { 

  /*while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  lastRecvTime = millis(); //Here we receive the data
  }*/
}