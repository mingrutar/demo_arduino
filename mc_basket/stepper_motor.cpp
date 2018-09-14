//
// stepper_motor.cpp
//

#include "stepper_motor.h"

StepperMotor::StepperMotor() {
  // encoder
  pinMode(PIN_ENCODER_CLK,INPUT);
  pinMode(PIN_ENCODER_DT,INPUT);
  pinMode(PIN_ENCODER_SW,INPUT);
  digitalWrite(PIN_ENCODER_SW, HIGH);     // Pull-Up resistor for switch
  attachInterrupt (20, isr, FALLING);      // interrupt 0 always connected to pin 2 on Arduino UNO
  count_down = 0;
  enable_encoder = false;
  enable_IR = false;
  mystepper.setSpeed(MaxStepSpeed);
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
        DeviceBase::pLEDIndicator->process(LED_STEPPER_IR_ONOFF | LED_INDICATOR_OFF);
        enable_encoder = !enable_encoder;
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
void StepperMotor::clean() {
  count_down = 0;
  digitalWrite(PIN_STEPPER_COILIN1, LOW);
  digitalWrite(PIN_STEPPER_COILIN2, LOW);
  digitalWrite(PIN_STEPPER_COILIN3, LOW);
  digitalWrite(PIN_STEPPER_COILIN4, LOW);
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
//      mystepper.step(-stepsPerRevolution);
    } else if (count_down == 0) {
      unlock_display();
      clean();
    }
  } else if (enable_encoder && turned) {
    run_step(stepToTake);
//    mystepper.step(stepToTake * (rotationdirection ? -1 : 1));
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
