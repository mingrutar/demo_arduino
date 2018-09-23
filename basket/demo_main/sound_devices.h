
//#include <pitches.h>
#include "led_indicator.h"

/// sound
static const int SOUND_LIMIT = 200;
static const int LED_DURATION = 1;
/*
 * class SoundDevices
 */
class SoundDevices : public DeviceBase {
  unsigned int count_down = 0;

public:
  SoundDevices();
  virtual int process(int opt=0);
  virtual void updateTime();
  virtual void clean() {;};
};
////////
SoundDevices::SoundDevices() {
//  num_tone = sizeof(melody) / sizeof(int);
}
int SoundDevices::process(int opt) {
  if (count_down == 0) {                // ignore sound while play tone
    int sensorValue  = analogRead(PIN_SOUND_ANALOG);
    if (sensorValue > SOUND_LIMIT) {
      DeviceBase::pLEDIndicator->process(LED_SOUND_HEARD | LED_INDICATOR_ON);
      count_down = LED_DURATION;
    }
  }
  return opt;
}
void SoundDevices::updateTime() {
  if (count_down > 0) {
    --count_down;
    if (count_down == 0) {
      DeviceBase::pLEDIndicator->process(LED_SOUND_HEARD | LED_INDICATOR_OFF);
    }
  }
}
