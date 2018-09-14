//
// dcmotor.h
//
#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "led_indicator.h"

// pins
static const char* modes[] = {"inactive", "forward", "reverse", "slow stop", "other way", "fast stop", "pause"};
static const int forward_reverse[] = {1, 2, 0};
static const int fast_slow[] = {1, 3, 4, 5, 0};
static const int run_all[] = {1, 2, 6, 3, 4, 5, 1, 2, 0};

extern void show_msg(const char* msg, int ln, bool lock);
extern void unlock_display();

class DCMotorFan : public DeviceBase {

  // consts
  const long TIME_RUN = 2;            // N sec spine the fan
  const long TIME_SLOW_RUN = 3;       // N sec spine the fan
  const long TIME_PAUSE = 5;          // Nsec Between options
  // run mode and its name

  unsigned int count_down = 0;
  const int* prun;

public:
  DCMotorFan();
  ~DCMotorFan() {
    write_out(LOW, LOW, LOW);
  };
  virtual int process(int );
  virtual void updateTime();

private:
  void run_fan();
  void write_out(int enable, int dira, int dirb);
  void write_out(int dira, int dirb);
};
#endif   // DCMOTOR_H
