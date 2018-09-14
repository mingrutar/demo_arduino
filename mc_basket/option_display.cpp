//
// option_display.h
//
#include "option_display.h"

static const char* Messages[] = {"Hello", "Bravo", "Good Job", "Smile", "Great", "Have Fun"};
static const char* Options[] = {"Show this", "DCM_Fan Fwd+Rvs", "DCM_Fan Fst2Slw", "DCM_Fan All", \
                  "Stepper M Auto", "Stepper M IR", "Stepper M Step", "Search & Smile"};
static const char* stepper_up_down[] = {"turn left,wait", "turn right,wait"};

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
void OptionSelector::clean() {
  count_down = 0;
  show_menu = false;
  msg_expire_time = 0;
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
int OptionSelector::num_options() {
  return sizeof(Options) / sizeof(Options[0]);
}
int OptionSelector::num_quotes() {
  return sizeof(Messages) / sizeof(Messages[0]);
}
