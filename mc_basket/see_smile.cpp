//
// see_smile.cpp
//
#include "see_smile.h"

SeeSmile::SeeSmile(LEDMatrix* leds) {
  enabled = false;
  Serial.println("SeeSmile::SeeSmile()");
  pinMode(PIN_HC_SR501_PIR, INPUT);

//  myservo.attach(PIN_SERVO_SG90_PULSE); // attaches the servo on pin 9 to the servo object
//  myservo.write(0);                     // back to 0 position
//  delay(SERVO_DELAY);                   // wait for a sec
  Serial.println("SeeSmile::SeeSmile()2");
  led_matrix = leds;
}
int SeeSmile::process(int ) {
  enabled = !enabled;
  if (!enabled) {
    clean();
    led_matrix->process(LEDS_RANDOM);
    DeviceBase::pLEDIndicator->process(LED_SEE_SMILE_ONOFF | LED_INDICATOR_OFF);
    DeviceBase::pLEDIndicator->process(LED_MONITOR_DETECTED | LED_INDICATOR_OFF);
    DeviceBase::pLEDIndicator->process(LED_ULTRASOUND_DETECTED | LED_INDICATOR_OFF);
  } else {
    DeviceBase::pLEDIndicator->process(LED_SEE_SMILE_ONOFF | LED_INDICATOR_ON);
    myservo.write(SERVO2POS);
    delay(SERVO_DELAY * 2);
  }
}
void SeeSmile::updateTime() {
  if (enabled) {
    DeviceBase::pLEDIndicator->process(LED_MONITOR_DETECTED | LED_INDICATOR_OFF);
    DeviceBase::pLEDIndicator->process(LED_ULTRASOUND_DETECTED | LED_INDICATOR_OFF);
    int pirValue = digitalRead(PIN_HC_SR501_PIR);
    if (pirValue == 1) {  // detect
      DeviceBase::pLEDIndicator->process(LED_MONITOR_DETECTED | LED_INDICATOR_ON);
      do_search(SERVO_N_TURN);
    }
  }
}
void SeeSmile::do_search(int n_times) {
  bool bSearch = true;
  myservo.attach(PIN_SERVO_SG90_PULSE); // attaches the servo on pin 9 to the servo object
  delay(SERVO_DELAY);
  for (int i = 0; (i < n_times) && bSearch; i++) {
    Serial.print("SeeSmile::do_search, i=");
    Serial.println(i);
    bSearch = !turn_search(0, 180, SERVO_TURN_DEGREE);
    if (bSearch) {
      bSearch = !turn_search(180, 0, -SERVO_TURN_DEGREE);
    }
  }
  clean();
}
bool SeeSmile::turn_search(int start, int end, int offset) {
  int pos = start;
  long distance_cm(0);
  for (int pos = start; pos < end; pos+=offset) {
    myservo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(SERVO_DELAY);               // waits 15ms for the servo to reach the position

    long distance_cm = sr04.Distance();
    if ((distance_cm <= SR04_MAX_DISTANCE) && (distance_cm > 0)) {
      DeviceBase::pLEDIndicator->process(LED_ULTRASOUND_DETECTED | LED_INDICATOR_ON);
      Serial.print("SeeSmile::turn_search, distance_cm=");
      Serial.println(distance_cm);
      led_matrix->process(LEDS_SMILE);
      return true;
    }
  }
  return false;
}
void SeeSmile::clean() {
  myservo.write(SERVO2POS);
  delay(SERVO_DELAY * 2);
  myservo.detach();
}
