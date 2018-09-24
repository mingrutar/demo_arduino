/**
  device_base.h
  abstract class DeviceBase()

*/
#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#define MAX_DURATION_SEC 2147483647L

//// Pins
// Sound Sensor
const int PIN_SOUND_ANALOG =  A0;     // select the input  pin for  the potentiometer

// LCD, L22, LiquidCrystal lcd(4, 5, 6, 7, 8, 9);  // (7, 8, 9, 10, 11, 12);
const int PIN_LCD_RS = 4;    // was 7, A register select pin that controls where in the LCD’s memory
                         // you are writing data to. You can select either the data register,
                         // which holds what goes on the screen, or an instruction register, which
                         // is where the LCD’s controller looks for instructions on what to do next.
const int PIN_LCD_E  = 5;    // was 8, A pin that adjust the contrast of LCD1602
const int PIN_LCD_DB4 = 6;   // was 9, Pins that read and write data
const int PIN_LCD_DB5 = 7;   // was 10
const int PIN_LCD_DB6 = 8;   // was 11
const int PIN_LCD_DB7 = 9;   // was 12

// Servo SG90 L9
const int PIN_SERVO_SG90_PULSE = 10;    // was 9

// ultrasound sensor SR04, L10
const int PIN_SR04_ECHO = 11;
const int PIN_SR04_TRIG = 12;

/// IR receiver, L14
const int PIN_IR_RECEIVER = 22;         // was 11

// LED Matrix, L15
const int PIN_LED_MATRIX_DATAIN = 23;    // DataIn, was 12
const int PIN_LED_MATRIX_CS = 24;        // LOAD(CS), was 11
const int PIN_LED_MATRIX_CLK = 25;       // CLK, was 10

// 74HC595, 8 LED, L24
const int PIN_74HC595_DATA= 26;          // DS, orange, was 12
const int PIN_74HC595_LATCH = 27;        // ST_CP, white, was 11
const int PIN_74HC595_CLOCK= 29;         // SH_CP, blue, was 9

// Motion, HC-SR501 PIR Sensor, L17
const int PIN_HC_SR501_PIR = 28;         // Output for HC-S501

// DC Motor, L29
const int PIN_L293D_DIRA = 32;
const int PIN_L293D_DIRB = 33;
const int PIN_L293D_ENABLE = 34;

// Stepper Motor ULN2003 Driver Board, L31
const int PIN_STEPPER_COILIN1 = 38;
const int PIN_STEPPER_COILIN2 = 39;
const int PIN_STEPPER_COILIN3 = 40;
const int PIN_STEPPER_COILIN4 = 41;

// Stepper Motor with encoder L33
const int PIN_ENCODER_CLK = 43;   // Generating interrupts using CLK signal, was ?
const int PIN_ENCODER_DT = 44;    // Reading DT signal, was ?
const int PIN_ENCODER_SW = 45;    // Reading Push Button switch, was ?

// Membrane Switc, L11
const int PIN_KEYPAD_D8 = 46;               // was 2
const int PIN_KEYPAD_D7 = 47;               // was 3
const int PIN_KEYPAD_D6 = 48;               // was 4
const int PIN_KEYPAD_D5 = 49;               // was 5
const int PIN_KEYPAD_D4 = 50;               // was 6
const int PIN_KEYPAD_D3 = 51;               // was 7
const int PIN_KEYPAD_D2 = 52;               // was 8
const int PIN_KEYPAD_D1 = 53;               // was 9

/// Key Mapping
const int KEY_INVALID = -2;
const int NON_INPUT = -1;

const int KEY_0 = 0;          // inactive
const int KEY_1 = 1;          // DC motor
const int KEY_2 = 2;
const int KEY_3 = 3;          // DC motor all
const int KEY_4 = 4;          // Stepper motor
const int KEY_5 = 5;
const int KEY_6 = 6;          // Stepper motor, not work
const int KEY_7 = 7;          // enable/disable see-smile
// const int KEY_8 = 8;
// const int KEY_9 = 9;
const int KEY_PLUS = 21;
const int KEY_MINUS = 22;

//// 74HC595 LED indicators
const byte LED_SEE_SMILE_ONOFF = 0;        //
const byte LED_STEPPER_IR_ONOFF = 2;       //
const byte LED_STEPPER_ENCODER_ONOFF = 1;  // #3 LED not working
const byte LED_SOUND_HEARD = 3;
const byte LED_MONITOR_DETECTED = 4;       //
const byte LED_ULTRASOUND_DETECTED = 5;
const byte LED_STEPPER_TURNING = 6;
const byte LED_FAN_TURNING = 7;

const short LED_INDICATOR_ON  = 0x0100;
const short LED_INDICATOR_OFF  = 0x0000;        // high byte off


/// constants
const int MAX_LCD_LEN = 16;
const int LEDS_SMILE = 0;
const int LEDS_RANDOM = 1;

/**
 * class DeviceBase
 */
class LED_Indicator;
class DeviceBase {
public:
  DeviceBase() {;};

  virtual int process(int ) = 0;
  virtual void updateTime() = 0;
  virtual void clean() {;};
  static LED_Indicator *pLEDIndicator;
};
#endif  // DEVICE_BASE_H
