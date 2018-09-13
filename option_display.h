///
//  option_display.h
//
// classes of demo devices
//
//// Options and Displayers
#ifndef LIQUID_CRYSTAL_H
#include <LiquidCrystal.h>
#define LIQUID_CRYSTAL_H
#endif
#include "device_base.h"

/// LCD
static LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);  // (7, 8, 9, 10, 11, 12);

static const char* Messages[] = {"Hello", "Bravo", "Good Job", "Smile", "Great", "Have Fun"};
static const char* Options[] = {"Show this", "DCM_Fan Fwd+Rvs", "DCM_Fan Fst2Slw", "DCM_Fan All", \
                  "Stepper M Auto", "Stepper M IR", "Stepper M Step", "Search & Smile"};
static const char* stepper_up_down[] = {"turn left,wait", "turn right,wait"};

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
  inline int num_options() {return sizeof(Options) / sizeof(Options[0]);}
  inline int num_quotes() {return sizeof(Messages) / sizeof(Messages[0]);}
};
////////////////////////
OptionSelector::OptionSelector(const char* name) {
//  Options = options;
//  Messages = quotes;
  count_down = 0;
  show_menu = false;
  username = name;
  sprintf(DEFAULT_L1, "Enter a key 0-%d", num_options()-1);
  lcd.begin(16, 2);
  Serial.println(name);
  Serial.println(DEFAULT_L1);
  printHelp(0);
}
int OptionSelector::process(int sel_option) {
  int ret = sel_option;
  if (sel_option == KEY_INVALID) {
    if (!lock_display) {
      count_down = TIME_DELAY_ERROR;
      msg_expire_time = 0;
      sprintf(msgbuff[0], "1-%d only", num_options()-1);
      sprintf(msgbuff[1], "try again, %2d s", TIME_DELAY_ERROR);
    }
    ret = NON_INPUT;
  } else if (sel_option == KEY_0) {
    showAllOptions();
  } else {
    count_down = 0;
    msg_expire_time = TIME_DELAY_CHANGE;
    unlock_display();
    if (sel_option < num_options()) {
      sprintf(msgbuff[0], "Option %d:", sel_option);
      sprintf(msgbuff[1], "%s", Options[sel_option]);
    } else if ((sel_option == KEY_PLUS) || (sel_option == KEY_MINUS)) {
      sprintf(msgbuff[1], "%s", stepper_up_down[(sel_option == KEY_MINUS) ? 0 : 1]);
    }
  }
  display_lines();
  return ret;
}
void OptionSelector::clean() {
  count_down = 0;
  lcd.clear();
}

void OptionSelector::printHelp(long sel) {
  strcpy(this->msgbuff[0], DEFAULT_L1);
  long randNumber = sel == -1 ? random(num_quotes()) : sel;
  sprintf(this->msgbuff[1], "%s %s", Messages[randNumber], username);
  this->display_lines();
  msg_expire_time = TIME_INTERVAL;
  count_down = 0;
}
void OptionSelector::showAllOptions() {
  memset(tmpbuf, ' ', MAX_LCD_LEN);
  tmpbuf[MAX_LCD_LEN] = '\0';
  for (int i= 0; i < num_options(); i++) {
    if (i == 0) {
      sprintf(msgbuff[0], "Key %d: show", i);
    } else if ((i >1) && (i < 5)) {
      sprintf(msgbuff[0], "Key %d: run", i);
    } else {
      sprintf(msgbuff[0], "Key %d: toggle", i);
    }
    sprintf(msgbuff[1], "%s", Options[i]);
    display_lines();
    delay(SHOW_HELP_INTERVAL);
  }
}

void OptionSelector::updateTime() {
  if (this->count_down > 0) {
      Serial.print("OptionSelector::updateTime: count_down=");
      Serial.println(count_down);
    sprintf(this->msgbuff[1], "try again, %2d s", count_down);
    this->display_lines();
    --this->count_down;
  } else if (msg_expire_time > 0) {
    --msg_expire_time;
  }
  if ((this->count_down == 0) && (msg_expire_time == 0) && !lock_display) {
    this->printHelp();
  }
}
void OptionSelector::display_lines() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
//    Serial.println("display_lines, msgbuff=");
//    Serial.println(msgbuff[i]);
    lcd.setCursor(0, i);
    lcd.print(msgbuff[i]);
  }
}
