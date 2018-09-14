//
// led_indicator.cpp
//
#include "led_indicator.h"

LED_Indicator::LED_Indicator() {
  pinMode(PIN_74HC595_LATCH, OUTPUT);
  pinMode(PIN_74HC595_DATA, OUTPUT);
  pinMode(PIN_74HC595_CLOCK, OUTPUT);
  updateShiftRegister(0);
  delay(SHORT_DELAY_TIME);
}
void LED_Indicator::clean() {
  updateShiftRegister(0);
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
