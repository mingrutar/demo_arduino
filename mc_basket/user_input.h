//
// user_input.h
//
/// IR Receiver
//  www.elegoo.com
// 2016.12.9
#ifdef USER_INPUT_H
#define USER_INPUT_H

#include <Keypad.h>
#include <IRremote.h>

#include "device_base.h"

static const int TIME_DELAY = 500;

/// IR receiver
static IRrecv irrecv(PIN_IR_RECEIVER);    // create instance of 'irrecv'

/**
 * class UserInput
 */
class  UserInput : public DeviceBase {
  unsigned int count_down;
  decode_results results;

public:
  UserInput();
  virtual ~UserInput() {;};
  virtual int process(int opt=NON_INPUT);
  virtual void updateTime();
  virtual void clean();

private:
  int readIR();                 // get IR code received
  int readKeyPad(char );                  // read key pad
};
#endif          // USER_INPUT_H
