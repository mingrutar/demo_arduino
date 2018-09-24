//www.elegoo.com
//2016.12.08
#include <Servo.h>
#include <SR04.h>
#include "led_indicator.h"

// motion detector: HC-SR501 PIR Sensor; 110 degree; 3[-7] m, 3sec[-5min]
static const int MIN_PIR_VALUE = 1;    //??

// Servo SG90 L9, twelve servo objects can be created on most boards
static const int SERVO_DELAY = 15;     // was 15 for 1 degree
static Servo myservo;  // create servo object to control a servo

// ultrasound sensor SR04, L10
// const int SR04_DELAY = 60;
static SR04 sr04 = SR04(PIN_SR04_ECHO, PIN_SR04_TRIG);  // ultrasound

static const int SR04_MAX_DISTANCE = 100;    // up to 400cm, but we want .5 m
static const int SERVO_N_TURN = 3;           // search 3 times;
static const int SERVO_TURN_DEGREE = 2;
static const int SERVO2POS = 0;              //rest at 0
static const int HOLD_DURATION = 10;         // hold for N sec

//LEDMatrix leds = LEDMatrix();

class SeeSmile : public DeviceBase {
  LEDMatrix* led_matrix;
  unsigned int count_down;
  bool enabled;
public:
  SeeSmile(LEDMatrix* leds);
  virtual int process(int );
  virtual void updateTime();
  virtual void clean();

private:
  bool turn_search(int startpos, int endpos, int offset);
  void do_search(int n_times);
};

////
SeeSmile::SeeSmile(LEDMatrix* leds) {
  enabled = false;
//  Serial.println("SeeSmile::SeeSmile()");
  pinMode(PIN_HC_SR501_PIR, INPUT);

//  myservo.attach(PIN_SERVO_SG90_PULSE); // attaches the servo on pin 9 to the servo object
//  myservo.write(0);                     // back to 0 position
//  delay(SERVO_DELAY);                   // wait for a sec
  Serial.println("SeeSmile::SeeSmile()");
  led_matrix = leds;
  count_down = 0;
  clean();
}
void SeeSmile::clean() {
  myservo.write(SERVO2POS);
  delay(SERVO_DELAY * 2);
  myservo.detach();
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
    Serial.print("SeeSmile::updateTime, count_down=");
    Serial.println(count_down);
    DeviceBase::pLEDIndicator->process(LED_MONITOR_DETECTED | LED_INDICATOR_OFF);
    DeviceBase::pLEDIndicator->process(LED_ULTRASOUND_DETECTED | LED_INDICATOR_OFF);
    if (count_down > 0) {
      count_down--;
      if (count_down == 0)
          led_matrix->process(LEDS_RANDOM);
    }
    if (count_down == 0) {
      int pirValue = digitalRead(PIN_HC_SR501_PIR);       // motion sensor
      if (pirValue == 1) {  // detect
        DeviceBase::pLEDIndicator->process(LED_MONITOR_DETECTED | LED_INDICATOR_ON);
        do_search(SERVO_N_TURN);
      }
    }
  }
}
void SeeSmile::do_search(int n_times) {
  bool bSearch = true;
  myservo.attach(PIN_SERVO_SG90_PULSE); // attaches the servo on pin 9 to the servo object
  delay(SERVO_DELAY);
  for (int i = 0; (i < n_times) && bSearch; i++) {
//    Serial.print("SeeSmile::do_search, i=");
//    Serial.println(i);
    bSearch = !turn_search(0, 180, SERVO_TURN_DEGREE);
    if (bSearch) {
      bSearch = !turn_search(180, 0, -SERVO_TURN_DEGREE);
    }
  }
  clean();      // detach servo
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
//      Serial.print("SeeSmile::turn_search, distance_cm=");
//      Serial.println(distance_cm);
      led_matrix->process(LEDS_SMILE);
      count_down = HOLD_DURATION;       // found something
      return true;
    }
  }
  return false;
}
