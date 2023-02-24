/* Tranmsitter code for the Arduino Radio control with PWM output
 * Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO, NANO, Pro mini (5V,16MHz)
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO,NANO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

This code transmits 1 channels with data from pins A0 POTENTIOMETER
Please, like share and subscribe : https://www.youtube.com/c/ELECTRONOOBS
*/
#include <OledDisplay.h>
#include <RCChannel.h>
#include <BatteryLife.h>
#include <ButtonState.h>
#include <RFCommunication.h>
#include <SPI.h>
#include <RF24.h>
#include <EEPROM.h>

const uint64_t pipeA = 0xE8E8F0F0E1LL; //Remember that this code should be the same for the receiver
const byte VIN_C1 = A8;
const byte LED = 11;
/* Coefficient diviseur du pont de résistance */
const float COEFF_PONT_DIVISEUR_VIN_C1 = 1.05;

//Data_to_be_sent sent_data;
RF24 radio(9, 10);
OledDisplay displayOled;
RCChannel rc;
BatteryLife batLife;
ButtonState btnState;
RFCommunication rfcom;
//int a;

void setup() {
  Serial.begin(9600);
  pinMode(VIN_C1, INPUT);
  pinMode(LED, OUTPUT);
  // digitalWrite(A8, HIGH);

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS); //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.openWritingPipe(pipeA); 
  radio.setChannel(120); //2400 + 120 = 2520MHz 76 default

  digitalWrite(LED, HIGH);

}

/** Mesure la référence interne à 1.1 volts */
unsigned int analogReadReference(void) {
  
  /* Elimine toutes charges résiduelles */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4F;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#endif
  delayMicroseconds(5);
  
  /* Sélectionne la référence interne à 1.1 volts comme point de mesure, avec comme limite haute VCC */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4E;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#endif
  delayMicroseconds(200);

  /* Active le convertisseur analogique -> numérique */
  ADCSRA |= (1 << ADEN);
  
  /* Lance une conversion analogique -> numérique */
  ADCSRA |= (1 << ADSC);
  
  /* Attend la fin de la conversion */
  while(ADCSRA & (1 << ADSC));
  
  /* Récupère le résultat de la conversion */
  return ADCL | (ADCH << 8);
}

void loop() {
  /* Mesure la tension en VIN et la référence interne à 1.1 volts */
  unsigned int raw_vin_c1 = analogRead(VIN_C1);
  unsigned int raw_ref = analogReadReference();
  float tension_low = 3.0;
  float tension_high = 4.2;
  /* Calcul de la tension réel avec un produit en croix */
  float real_vin_c1 = ((raw_vin_c1 * 1.1) / raw_ref) * COEFF_PONT_DIVISEUR_VIN_C1;
  float real_vin_per = (real_vin_c1-tension_low)/(tension_high-tension_low)*100;
  float battery_life = 21*real_vin_per/100;
  Serial.print("Cell 1 (V) : ");Serial.print(real_vin_c1, 3);
  Serial.print("Cell 1 (%) : ");Serial.print(real_vin_per, 3);
  Serial.print("Battery Life : ");Serial.println(battery_life, 1);
  
  batLife.updateBatteryLife (battery_life);
  btnState.updateBtnState ();
  rc.updateAnalogChannel (analogRead(A2),analogRead(A3),analogRead(A0),analogRead(A1));
  displayOled.screen (&btnState, &rc, &batLife);
  rfcom.encodeData (&radio, &btnState, &rc);

  /* ùupdateChannel();
  a = EEPROM.read( 55 );
  Serial.print ("EEPROM TH = ");
  Serial.print (a);
  Serial.print (" | ");
  */
  
}
