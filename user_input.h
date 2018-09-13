#include <Stepper.h>

/// IR Receiver
//  www.elegoo.com
// 2016.12.9

#include <Keypad.h>
#include <IRremote.h>

#include "device_base.h"

static const int TIME_DELAY = 500;

/// IR receiver
static IRrecv irrecv(PIN_IR_RECEIVER);    // create instance of 'irrecv'

/// Keypad
static const byte ROWS = 4; //four rows
static const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads
static char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}     // '* => 5, '*' => 6
};
static byte rowPins[ROWS] = {PIN_KEYPAD_D1, PIN_KEYPAD_D2, PIN_KEYPAD_D3, PIN_KEYPAD_D4};  // was {9, 8, 7, 6} //connect to the row pinouts of the keypad
static byte colPins[COLS] = {PIN_KEYPAD_D5, PIN_KEYPAD_D6, PIN_KEYPAD_D7, PIN_KEYPAD_D8};  // was {5, 4, 3, 2} //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
static Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
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
//////////////
UserInput::UserInput() {
  count_down = 0;
  Serial.println("UserInput");
  irrecv.enableIRIn();                // Start the receiver
}
void UserInput::clean() {
  count_down = 0;  
}
int UserInput::process(int opt) {
  int ret = NON_INPUT;
  char inputchar = customKeypad.getKey();
  if (inputchar) {
    ret = readKeyPad(inputchar);
  } else {          // get IR
    ret = readIR();
  }
  // do not accept input for valid key
  if ((ret != NON_INPUT) && (ret != KEY_INVALID)) {
    count_down =  TIME_DELAY;
    Serial.print("UserInput::process: ret=");
    Serial.println(ret);
  } else
    count_down = 0;
  return ret;
}
void UserInput::updateTime(void) {
  if (count_down > 0) {
    Serial.print("UserInput::updateTime: count_down=");
    Serial.println(count_down);
    --count_down;
  }
}
int UserInput::readKeyPad(char customKey) {
  int ret = NON_INPUT;
  if (isdigit(customKey)) {
    ret = customKey - '0';
    if ((ret < KEY_0) || (ret > KEY_7))
      ret = KEY_INVALID;
  } else if (customKey == '*') {           // 5 or * => step rotate forward
    ret = KEY_MINUS;
  } else if (customKey == '#') {           //  or # => step rotate backforward
    ret = KEY_PLUS;
  } else {
    Serial.print("readKeyPad, KEY_INVALID: customKey=");
    Serial.println(customKey);
    ret = KEY_INVALID;
  }
  return ret;
}
// describing Remote IR codes
const int MAX_REPEAT = 3;
int UserInput::readIR() {
  int ret = NON_INPUT;
  int repeat = MAX_REPEAT;
  bool bFound;
  do {
    bFound = true;
    if (irrecv.decode(&results)) {      // have we received an IR signal?
      switch(results.value) {
        case 0xFF22DD:                  // Serial.println("FAST BACK");
        case 0xFFE01F:                  // Serial.println("DOWN");
          ret = KEY_MINUS;
          break;
        case 0xFFC23D:                  // Serial.println("FAST FORWARD");
        case 0xFF906F:                  // Serial.println("UP");
          ret = KEY_PLUS;
          break;
        case 0xFF6897:
          ret = KEY_0;
          break;
        case 0xFF30CF:                   //Serial.println("1");    break;
          ret = KEY_1;
          break;
        case 0xFF18E7:                  // Serial.println("2");    break;
          ret = KEY_2;
          break;
        case 0xFF7A85:
          ret = KEY_3;
          break;
        case 0xFF10EF:
          ret = KEY_4;
          break;
        case 0xFF38C7:
          ret = KEY_5;
          break;
        case 0xFF5AA5:
          ret = KEY_6;
          break;
        case 0xFF42BD:
          ret = KEY_7;
          break;
//  case 0xFF4AB5: Serial.println("8");    break;
//  case 0xFF52AD: Serial.println("9");    break;
        case 0xFFFFFFFF:              // repeat up to 10 times
          repeat--;
          bFound = false;
          break;
        default:
          Serial.print("readIR, KEY_INVALID: results.value=");
          Serial.println(results.value, HEX);
          ret = KEY_INVALID;           //Serial.println(" other button   ");
      }
      irrecv.resume(); // receive the next value
    }
  } while (!bFound && (repeat > 0));
//  Serial.print("irrecv, code=");
//  Serial.println(code, HEX);
  return ret;
}
