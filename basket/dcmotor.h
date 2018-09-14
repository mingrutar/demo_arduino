//
// dcmotor.h
//
#include "led_indicator.h"

// pins
static const char* modes[] = {"inactive", "forward", "reverse", "slow stop", "other way", "fast stop", "pause"};
static const int forward_reverse[] = {1, 2, 0};
static const int fast_slow[] = {1, 3, 4, 5, 0};
static const int run_all[] = {1, 2, 6, 3, 4, 5, 1, 2, 0};

extern void show_msg(const char* msg, int ln, bool lock);
extern void unlock_display();

class DCMotorFan : public DeviceBase {

  // consts
  const long TIME_RUN = 2;            // N sec spine the fan
  const long TIME_SLOW_RUN = 3;       // N sec spine the fan
  const long TIME_PAUSE = 5;          // Nsec Between options
  // run mode and its name

  unsigned int count_down = 0;
  const int* prun;

public:
  DCMotorFan();
  ~DCMotorFan() {
    write_out(LOW, LOW, LOW);
  };
  virtual int process(int );
  virtual void updateTime();
  virtual void clean();

private:
  void run_fan();
  void write_out(int enable, int dira, int dirb);
  void write_out(int dira, int dirb);
};

////////////////
DCMotorFan::DCMotorFan() {
  pinMode(PIN_L293D_ENABLE, OUTPUT);
  pinMode(PIN_L293D_DIRA, OUTPUT);
  pinMode(PIN_L293D_DIRB, OUTPUT);
  prun = NULL;
}
void DCMotorFan::clean() {
  count_down = 0;
  write_out(LOW, LOW, LOW);
  prun = NULL;
}

int DCMotorFan::process(int opt) {
  write_out(LOW, LOW, LOW);         // turn off first
  if (opt == KEY_1) {
//    Serial.print("DCMotorFan::run_fan: forwardReverse");
    prun =  &forward_reverse[0];
  } else if (opt ==  KEY_2) {
//    Serial.print("DCMotorFan::run_fan: fastSlow");
    prun = &fast_slow[0];
  } else if (opt == KEY_3) {
//    Serial.print("DCMotorFan::run_fan: all");
    prun = &run_all[0];
  } else {
    DeviceBase::pLEDIndicator->process(LED_FAN_TURNING | LED_INDICATOR_OFF);
    write_out(LOW, LOW, LOW);
    prun = NULL;
  }
  if (prun)
    DeviceBase::pLEDIndicator->process(LED_FAN_TURNING | LED_INDICATOR_ON);
    run_fan();
  return opt;
}
void DCMotorFan::updateTime() {
  if (count_down > 0) {
    Serial.print("DCMotor_Fan::updateTime: count_down=");
    Serial.println(count_down);
    --count_down;
  } else if (prun != NULL) {
    DeviceBase::pLEDIndicator->process(LED_FAN_TURNING | LED_INDICATOR_ON);
    run_fan();
  }
}
void DCMotorFan::write_out(int enable, int dira, int dirb) {
  digitalWrite(PIN_L293D_ENABLE, enable); // enable on
  digitalWrite(PIN_L293D_DIRA, dira); //one way
  digitalWrite(PIN_L293D_DIRB, dirb);
}
void DCMotorFan::write_out(int dira, int dirb) {
  digitalWrite(PIN_L293D_DIRA, dira); //one way
  digitalWrite(PIN_L293D_DIRB, dirb);
}
void DCMotorFan::run_fan() {
  Serial.print("DCMotor_Fan::run_fan: *prun=");
  Serial.println(*prun);
  if (*prun == 0) {
    DeviceBase::pLEDIndicator->process(LED_FAN_TURNING | LED_INDICATOR_OFF);
    write_out(LOW, LOW, LOW);
    prun = NULL;
    unlock_display();
  } else {
    show_msg(modes[*prun], 1, true);
    count_down = TIME_RUN;
    switch(*prun) {
      case 1:
        write_out(HIGH, HIGH, LOW);
        count_down = TIME_SLOW_RUN;
        break;
      case 2:
        write_out(LOW, HIGH);
        break;
      case 3:
        digitalWrite(PIN_L293D_ENABLE, LOW); //slow stop
        break;
      case 4:
        write_out(HIGH, LOW, HIGH);  // one way
        count_down = TIME_SLOW_RUN;
        break;
      case 5:
        digitalWrite(PIN_L293D_DIRA,LOW); //fast stop
        break;
     case 6:
        write_out(LOW, LOW, LOW);
        count_down = TIME_PAUSE;
        break;
    default:
      write_out(LOW, LOW, LOW);
      prun = NULL;
      Serial.println("DCMotor_Fan::run_fan: unknown prun");
    }
    if (prun) {
      prun++;
    }
  }
}
