//www.elegoo.com
//2016.12.9


#include "device_base.h"
#include "option_display.h"
#include "dcmotor.h"
#include "user_input.h"
#include "sound_devices.h"
#include "stepper_motor.h"
#include "led_matrix.h"
#include "led_indicator.h"
#include "see_smile.h"

/// App consts

static const char MYNAME[] = "Ming";
static const int NAME_LEN = 7;

static int sel_option = 1;
static char namebuff[NAME_LEN];

static long lmsec_val=0, msec_val;
static int ret_val = NON_INPUT;

static const int DEV_Input = 0;
static const int DEV_selector = 1;
static const int DEV_DCMotor = 2;
static const int DEV_StepperMotor = 3;
static const int DEV_SEE_SMILE = 4;
static const int DEV_LED_Matrix = 5;
static const int DEV_LED_Indicator = 6;
static const int DEV_Sound = 7;
static const int NUM_DEVICE = 8;

static DeviceBase* devices[NUM_DEVICE];
LED_Indicator* DeviceBase::pLEDIndicator = NULL;
static const char* dev_names[] = {"Input", "LCD", "DCMotor", "Stepper Motor",  \
                                 "Smile & See", "LED Matrix", "LED Indicator", "Sound"};
static bool switches[NUM_DEVICE] = {
  true,         // DEV_Input
  true,         // DEV_selector
  false,        // DEV_DCMotor
  false,        // DEV_StepperMotor
  false,        // DEV_SEE_SMILE
  true,         // DEV_LED_Matrix
  false,        // DEV_LED_Indicator
  true};        // DEV_Sound

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  for (int i = 0; i < NUM_DEVICE; i++) {
    devices[i] = NULL;
  }
  // set up the LCD's number of columns and rows:
  randomSeed(analogRead(0));
  Serial.print("demomain:setup, switches are: ");
  for (int i = 0; i < NUM_DEVICE; i++) {
    Serial.print(dev_names[i]);
    Serial.print(" is ");
    Serial.print(switches[i] ? "on" : "off");
    Serial.println("\n          ");
  }
  devices[DEV_Input] =  new UserInput();              // 0 - input
  devices[DEV_DCMotor] = new DCMotorFan();            // - DCmotor
  devices[DEV_StepperMotor] = new StepperMotor();     // 4 - Stepper motor
  devices[DEV_selector] = new OptionSelector(MYNAME);
  devices[DEV_Sound] = new SoundDevices();
  LEDMatrix *leds = new LEDMatrix();
  devices[DEV_LED_Matrix] = leds;
  devices[DEV_SEE_SMILE] = new SeeSmile(leds);
  LED_Indicator* pIndicator = new LED_Indicator();
  DeviceBase::pLEDIndicator = pIndicator;
}
void loop(){
  devices[DEV_Sound]->process(NON_INPUT);
//  Serial.println("loop");

  ret_val = devices[DEV_Input]->process(NON_INPUT);             // read input
  if (ret_val != NON_INPUT) {
    Serial.print("loop, input ret_val=");
    Serial.println(ret_val);
    ret_val = devices[DEV_selector]->process(ret_val);   // verify the input
    if ((ret_val >= KEY_0) && (ret_val <= KEY_3) && switches[DEV_StepperMotor]) {
      devices[DEV_DCMotor]->process(ret_val);
    } else if (switches[DEV_StepperMotor] && ((ret_val==0) || ((ret_val >= KEY_4) && (ret_val <= KEY_6))  \
        || (ret_val == KEY_PLUS) || (ret_val == KEY_MINUS))) {
      devices[DEV_StepperMotor]->process(ret_val);
    } else if ((ret_val == KEY_7) && switches[DEV_SEE_SMILE]) {
      devices[DEV_SEE_SMILE]->process(KEY_7);
    } else {
      Serial.print("demomain: unknown key input=");
      Serial.println(ret_val);
    }
  }  
  // update time
  msec_val = millis()/1000;
  if ((msec_val- lmsec_val) > 0) {
    for (int i = 0; i < NUM_DEVICE; i++) {
      if (switches[i]) {
        devices[i]->updateTime();
      } else {
        devices[i]->clean();
      }
    }
    lmsec_val = msec_val;
  }
}
