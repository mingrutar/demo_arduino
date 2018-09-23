//
// stepper_motor.h
//
#include <Stepper.h>
#include <IRremote.h>
#include "led_indicator.h"

#define PIN_INTERRUPT 2

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
  Serial.print("isr, rotationdirection = ");
  Serial.println(rotationdirection ? "true" : "false");
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
  void resetMotor();
};
////////////
StepperMotor::StepperMotor() {
  // encoder
  pinMode(PIN_ENCODER_CLK,INPUT);
  pinMode(PIN_ENCODER_DT,INPUT);
  pinMode(PIN_ENCODER_SW,INPUT);
  pinMode(PIN_INTERRUPT, INPUT_PULLUP);
  digitalWrite(PIN_ENCODER_SW, HIGH);     // Pull-Up resistor for switch
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), isr, FALLING);      // interrupt 0 always connected to pin 2 on Arduino UNO
  count_down = 0;
  enable_encoder = false;
  enable_IR = false;
  mystepper.setSpeed(MaxStepSpeed);
}
void StepperMotor::clean() {
  resetMotor();
  count_down = 0;
  enable_encoder = false;
  enable_IR = false;
}
int StepperMotor::process(int opt) {
  Serial.print("StepperMotor::process, opt = ");
  Serial.println(opt);
  if (count_down == 0) {
    DeviceBase::pLEDIndicator->process(LED_STEPPER_TURNING | LED_INDICATOR_OFF);
    if (enable_IR && ((opt == KEY_PLUS) || (opt == KEY_MINUS))) {
      rotationdirection = (opt == KEY_PLUS);       // CCW=>rotationdirection true
      count_down = ONE_RUN;
      run_step(stepsPerRevolution);
    } else {
      if (opt == KEY_4) {                   // keep rotate for a duration
        enable_IR = false;
        DeviceBase::pLEDIndicator->process(LED_STEPPER_IR_ONOFF | LED_INDICATOR_OFF);
        enable_encoder = false;
        DeviceBase::pLEDIndicator->process(LED_STEPPER_ENCODER_ONOFF | LED_INDICATOR_OFF);
        count_down = AUTO_RUNS;
        rotationdirection = 1;
  //      mystepper.step(stepsPerRevolution);
        run_step(stepsPerRevolution);
      } else if (opt == KEY_5) {
        enable_encoder = false;
        DeviceBase::pLEDIndicator->process(LED_STEPPER_ENCODER_ONOFF | LED_INDICATOR_OFF);
        enable_IR = !enable_IR;
        show_msg(myMessages[enable_IR ? 0 : 1], 1, true);
        if (!enable_IR) {
          count_down = ONE_RUN;
          DeviceBase::pLEDIndicator->process(LED_STEPPER_IR_ONOFF | LED_INDICATOR_OFF);
        } else {
          DeviceBase::pLEDIndicator->process(LED_STEPPER_IR_ONOFF | LED_INDICATOR_ON);
        }
      } else if (opt == KEY_6) {
        count_down = 0;
        enable_IR = false;
        enable_encoder = !enable_encoder;
        DeviceBase::pLEDIndicator->process(LED_STEPPER_IR_ONOFF | LED_INDICATOR_OFF);
        Serial.print("enable_encoder=");
        Serial.println(enable_encoder ? "true" : " false");
        show_msg(myMessages[enable_encoder ? 2 : 3], 1, true);
        if (!enable_encoder) {
          count_down = ONE_RUN;                     // turn off next update
          DeviceBase::pLEDIndicator->process(LED_STEPPER_ENCODER_ONOFF | LED_INDICATOR_OFF);
        } else {
          DeviceBase::pLEDIndicator->process(LED_STEPPER_ENCODER_ONOFF | LED_INDICATOR_ON);
        }
      }
    }
  } else {
    Serial.print("StepperMotor is busy, ignored opt=");
    Serial.println(opt);
  }
  return opt;
}
void StepperMotor::updateTime() {
  if (count_down > 0) {
    Serial.print("StepperMotor::updateTime, count_down = ");
    Serial.println(count_down);
    --count_down;
    if (count_down > 0) {
      rotationdirection = -1;
      Serial.println("StepperMotor::updateTime counterclockwise");
      run_step(stepsPerRevolution);
    } else if (count_down == 0) {
      unlock_display();
      resetMotor();
    }
  } else if (enable_encoder && turned) {
    Serial.print("StepperMotor::enable_encoder && turned");
    run_step(stepToTake);
    turned = false;
  }
}
void StepperMotor::run_step(int num_steps) {
  Serial.print("StepperMotor rotationdirection=");
  Serial.println(rotationdirection ? "true" : "false");
  DeviceBase::pLEDIndicator->process(LED_STEPPER_TURNING | LED_INDICATOR_ON);
  mystepper.step(num_steps * (rotationdirection ? -1 : 1));
  DeviceBase::pLEDIndicator->process(LED_STEPPER_TURNING | LED_INDICATOR_OFF);
}
void StepperMotor::resetMotor() {
  digitalWrite(PIN_STEPPER_COILIN1, LOW);
  digitalWrite(PIN_STEPPER_COILIN2, LOW);
  digitalWrite(PIN_STEPPER_COILIN3, LOW);
  digitalWrite(PIN_STEPPER_COILIN4, LOW);
}
