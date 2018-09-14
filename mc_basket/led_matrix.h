//
// LED_matrix.h
//

//www.elegoo.com
//2016.12.9

#ifndef LED_MATRIX_H
#define LED_MATRIX_H
//We always have to include the library
#include "LedControl.h"
#include "device_base.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 23 is connected to the DataIn        // was 12
 pin 24 is connected to LOAD(CS)          // was 11
 pin 25 is connected to the CLK           // was 10
 We have only a single MAX72XX.
 */
static const int N_DEVICE = 1;

static const int N_SIZE = 8;
static const int N_SHORTBLINK = 4;
static const int N_LONGBLINK = 10;  // # blnk 2 circles
static const int N_DISPLAY = 11;
static const unsigned long DELAYTIME1 = 20;   //msec
static const unsigned long DELAYTIME2 = 50;   //msec

// LedControl(23, 25, 24) was LedControl(12,10,11,1);
static LedControl lc = LedControl(PIN_LED_MATRIX_DATAIN, PIN_LED_MATRIX_CLK, PIN_LED_MATRIX_CS, N_DEVICE);

class LEDDisply{
public:
  virtual void show() = 0;
};
static const byte smile[8]= {B00000000, B01100110, B00100100, B00000000, B01000010, B00100100, B00011000, B00000000};
static const byte heart[8]= {B01100110, B10011001, B10000001, B10000001, B10000001, B01000010, B00100100, B00011000};
static const byte big_o[8]= {B00011000, B01100110, B01000010, B10000001, B10000001, B01000010, B01100110, B00011000};
static const byte sad[8] =  {B00000000, B01100110, B00100100, B00000000, B00011000, B00100100, B01000010, B00000000};
static const byte look1[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B00000000, B01111110, B00000000};
static const byte look2[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B00000010, B01111100, B00000000};
static const byte look3[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B01111100, B00000010, B00000000};
static const byte look4[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B01000000, B00111110, B00000000};
static const byte look5[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B00111110, B01000000, B00000000};
static const byte look6[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B00111100, B01000010, B00000000};
static const byte look7[8]= {B00000000, B01100110, B00100100, B00000000, B00000000, B01000010, B00111100, B00000000};
// static const byte tree[8] = {B00011000, B00011000, B00111100, B01111110, B11111111, B10011001, B00011000, B00011000};

class Pattern : public LEDDisply {
  const byte* pdata;
public:
  Pattern(const byte* input) : pdata(input) {;};
  virtual void show();
};

class ByRow : public LEDDisply {
public:
  virtual void show();
};
class ByColumn : public LEDDisply {
public:
  virtual void show();
};
/*
 *
 */
class LEDMatrix :  public DeviceBase {
  unsigned int count_down = 0;
  int show_two[2];
  LEDDisply *display_arr[N_DISPLAY];
  int n_blink;
public:
  LEDMatrix();

  virtual int process(int opt);
  virtual void updateTime();
private:
  void show_random();
};
#endif
/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.

void columns() {
  for(int col = 0; col < N_SIZE; col++) {
    delay(DELAYTIME2);
    lc.setColumn(0, col, B10100000);
    delay(DELAYTIME2);
    lc.setColumn(0,col,(byte)0);
    for(int i=0; i<col;i++) {
      delay(DELAYTIME2);
      lc.setColumn(0,col,B10100000);
      delay(DELAYTIME2);
      lc.setColumn(0,col,(byte)0);
    }
  }
}
*/
/*
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.

void single() {
  for(int row = 0; row < N_SIZE; row++) {
    for(int col = 0; col < N_SIZE; col++) {
      delay(DELAYTIME2);
      lc.setLed(0,row,col, true);
      delay(DELAYTIME2);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(DELAYTIME2);
        lc.setLed(0,row,col,true);
        delay(DELAYTIME2);
      }
    }
  }
}
 */
/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix.
 (you need at least 5x7 leds to see the whole chars)

void writeArduinoOnMatrix() {
  lc.clearDisplay(0);
  delay(DELAYTIME2);

  // here is the data for the characters
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00010000,B00100000,B00100000,B00010000,B00111110};
  byte d[5]={B11111110,B00010010,B00100010,B00100010,B00011100};
  byte u[5]={B00111110,B00000100,B00000010,B00000010,B00111100};
  byte i[5]={B00000000,B00000010,B10111110,B00100010,B00000000};
  byte n[5]={B00011110,B00100000,B00100000,B00010000,B00111110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  // now display them one by one with a small delay
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  delay(delaytime1);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime1);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime1);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime1);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime1);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime1);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime1);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime1);
}
*/
