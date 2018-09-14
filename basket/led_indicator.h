//
// LED_matrix.h
//

//www.elegoo.com
//2016.12.9

#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H
//We always have to include the library
#include "LedControl.h"
#include "device_base.h"

// 74HC595, 8 LED, L24
// const int PIN_74HC595_DATA= 26;          // DS, orange, was 12
// const int PIN_74HC595_LATCH = 27;        // ST_CP, white, was 11
// const int PIN_74HC595_CLOCK= 29;         // SH_CP, blue, was 9

//// 74HC595 LED indicators
// const int LED_SEE_SMILE_ONOFF = 0;        //
// const int LED_STEPPER_IR_ONOFF = 1;       //
// const int LED_STEPPER_ENCODER_ONOFF = 2;  //
// const int LED_SOUND_HEARD = 3;
// const int LED_MONITOR_DETECTED = 4;       //
// const int LED_ULTRASOUND_DETECTED = 5;
// const int LED_STEPPER_TURNING = 6;
// const int LED_FAN_TURNING = 7;
//
// const byte LED_INDICATOR_ON  = 0x0100;
// const byte LED_INDICATOR_OFF  = 0x0000;        // high byte off
static const byte LED_INDICATOR_MASK = B00000111;     // last 3 are the enable/disable
const int SHORT_DELAY_TIME = 50;
const int LONG_DELAY_TIME = 500;

const int DELAY_TIME = 1;

class LED_Indicator : public DeviceBase {
  byte leds;

public:
  LED_Indicator();

  virtual int process(int opt);
  virtual void updateTime();
  virtual void clean();

private:
  byte turn_led(byte last, int pos, bool on);
  void updateShiftRegister(byte b);
};
/////
LED_Indicator::LED_Indicator() {
  leds = B0;
  pinMode(PIN_74HC595_LATCH, OUTPUT);
  pinMode(PIN_74HC595_DATA, OUTPUT);
  pinMode(PIN_74HC595_CLOCK, OUTPUT);
  updateShiftRegister(leds);
  delay(SHORT_DELAY_TIME);
}
void LED_Indicator::clean() {
  leds = B0;
  updateShiftRegister(leds);
  delay(SHORT_DELAY_TIME);
}
int LED_Indicator::process(int opt) {
  byte pos = opt & 0xFF;
  bool ison = (opt & 0xFF00 ) == LED_INDICATOR_ON;
  Serial.print("LED_Indicator::process: pos=");
  Serial.print(pos, BIN);
  Serial.print(", ison=");
  Serial.println(ison);

  leds = turn_led(leds, pos, ison);
  updateShiftRegister(leds);
  delay(SHORT_DELAY_TIME);
}
void LED_Indicator::updateTime() {
  updateShiftRegister(leds & LED_INDICATOR_MASK);
}
byte LED_Indicator::turn_led(byte last, int pos, bool on) {
  byte ret = 0;
  byte mybit = 1 << pos;
  if (on)
    ret = last | mybit;
   else
    ret = ~mybit & last;
  if (ret != last) {
    Serial.print("LED_Indicator::turn_led: ret=");
    Serial.print(ret, BIN);
    Serial.print(", last=");
    Serial.println(last, BIN);
  }
  return ret;
}
void LED_Indicator::updateShiftRegister(byte sr) {
  digitalWrite(PIN_74HC595_LATCH, LOW);
  shiftOut(PIN_74HC595_DATA, PIN_74HC595_CLOCK, MSBFIRST, sr);
  digitalWrite(PIN_74HC595_LATCH, HIGH);
}
#endif  // LED_INDICATOR_H
