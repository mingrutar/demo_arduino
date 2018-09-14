//www.elegoo.com
//2016.12.08
#ifndef SEE_SMILE_H
#define SEE_SMILE_H
#include <Servo.h>
#include <SR04.h>
#include "led_indicator.h"
#include "device_base.h"

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

//LEDMatrix leds = LEDMatrix();

class SeeSmile : public DeviceBase {
  LEDMatrix* led_matrix;
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

#endif  // SEE_SMILE_H
