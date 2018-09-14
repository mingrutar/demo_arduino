///
// option_display.h
//
// classes of demo devices
//

//// Options and Displayers
#ifndef LIQUID_CRYSTAL_H
#define LIQUID_CRYSTAL_H
#include <LiquidCrystal.h>
#include "device_base.h"

/// LCD
static LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);  // (7, 8, 9, 10, 11, 12);

static char tmpbuf[MAX_LCD_LEN + 1];
static unsigned int msg_expire_time = 0;

static const int TIME_DELAY_CHANGE = 25;   // N sec show the current selection
static const int TIME_DELAY_ERROR = 20;     // N sec prompt for enter a new option
static const int TIME_INTERVAL = 10;        // N sec change quotes
static const int SHOW_HELP_INTERVAL = 3000;  // N msec for help

static bool lock_display = false;
void show_msg(const char* msg, int ln, bool lock=true) {
  memset(tmpbuf, ' ', MAX_LCD_LEN);
  tmpbuf[MAX_LCD_LEN] = '\0';
  int n_len = strlen(msg);
  if (n_len >  MAX_LCD_LEN)
    n_len = MAX_LCD_LEN;
  memcpy(tmpbuf, msg, n_len);
//  Serial.print("show_msg: ");
//  Serial.println(msg);
  lcd.setCursor(0, (ln%2));
  lcd.print(tmpbuf);
  lock_display = lock;
}
void unlock_display() {
  lock_display = false;
  msg_expire_time = 0;
}
/*
 * class OptionSelector
 */
class OptionSelector : public DeviceBase {
  const char* username;

//  int num_option;
//  int num_str;
  char DEFAULT_L1[MAX_LCD_LEN + 1];
  unsigned int count_down = 0;
  char msgbuff[2][MAX_LCD_LEN+1];
  bool show_menu;

public:
  OptionSelector(const char* name);
  virtual ~OptionSelector() {;};
  virtual int process(int input_key);
  virtual void updateTime();
  virtual void clean();

private:
  void showAllOptions();
  void printHelp(long sel=-1);
  void display_lines();
  int num_options();
  int num_quotes();
};
#endif      // LIQUID_CRYSTAL_H
