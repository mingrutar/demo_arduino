//
// stepper_motor.h
//
#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <Stepper.h>
#include <IRremote.h>
#include "led_indicator.h"

static const int STEP = 32;     // Number of steps for one revolution of Internal shaft
                          // 2048 steps for one revolution of External shaft
static const int MaxStepSpeed = 200;        // 20, 500, 700
//const int maxManualStepSpeed = 500;     // 20, 500, 700

static const int stepToTake = 50;             // how much to turn
static const int stepsPerRevolution = 2048;    // 2048 = 1 Revolution change this to fit the number of steps per revolution

static const int AUTO_RUNS = 2;             // clockwise & counterclockwise
static const int ONE_RUN = 1;                // clockwise | counterclockwise

// initialize the stepper library on pins 8 through 11:
static bool rotationdirection;     // true - counterclockwise, false = clockwise
static bool turned = false;

extern void show_msg(const char* msg, int ln, bool lock);
extern void unlock_display();

static const char* myMessages[] = {"StepM IR On", "StepM IR Off", "Encoder On", "Encoder Off"};
static Stepper mystepper(STEP, PIN_STEPPER_COILIN1, PIN_STEPPER_COILIN3, PIN_STEPPER_COILIN2, PIN_STEPPER_COILIN4); //was 8, 10, 9, 11
//Stepper stepper_manual(STEP, PIN_STEPPER_COILIN1, PIN_STEPPER_COILIN3, PIN_STEPPER_COILIN2, PIN_STEPPER_COILIN4);   // was 8, 10, 9, 11

// Interrupt routine runs if CLK goes from HIGH to LOW
static void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(PIN_ENCODER_CLK))
    rotationdirection= digitalRead(PIN_ENCODER_DT);
  else
    rotationdirection= !digitalRead(PIN_ENCODER_DT);
//  Serial.print("isr, rotationdirection = ");
//  Serial.println(rotationdirection?"true":"false");
  turned = true;
}
/**
 * class StepperMotor
 */
class StepperMotor : public DeviceBase {
  unsigned int count_down;
  bool enable_encoder;
  bool enable_IR;
  char tmpbuf[MAX_LCD_LEN + 1];
  // int rotary_pos = 0;          // To store Stepper Motor Position
  // int prev_pos;                // Previous Rotary position Value to check accuracy
  // bool use_encoder;
public:
  StepperMotor();
  virtual int process(int opt);
  virtual void updateTime();
  virtual void clean();

private:
  void run_step(int num_steps);
};

#endif    // STEPPER_MOTOR_H
