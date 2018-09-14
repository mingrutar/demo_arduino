//
// cound_devices.h
//
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
  virtual void clean();

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
void SoundDevices::clean() {
  count_down = 0;
}
/////////////// used for demo on aug 18, single LED
// include the library code:
//#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//    if ((msec_val- led_msec_val) > LED_DELAY_TIME) {
//        setMyColor();
//        led_msec_val = msec_val;
//    }
//
/// pixel
// Define Arduino connection pin
/////void turnOffPixel() {
//  leds.setPixelColor(0, leds.Color(0, 0, 0));
//  leds.show();
//  delay(INTERVAL);
//}
//void setMyColor() {
//  uint32_t cval = getMyColor();
//  Serial.print("color val=");
//  Serial.println(cval, HEX);
//  leds.setPixelColor(0, cval);
////    colorWipe(leds.Color(MIDLIGHT, MIDLIGHT, MIDLIGHT), 50);
//  leds.show();
//  delay(INTERVAL);
//}
//uint32_t getMyColor() {
//  int val = random(0, 7);
//  if (val == 0) {
//    return leds.Color(MIDLIGHT, 0, 0);
//  } else if (val == 1) {
//    return leds.Color(0, MIDLIGHT, 0);
//  } else if (val == 2) {
//    return leds.Color(0, 0, MIDLIGHT);
//  } else if (val == 3) {
//    return leds.Color(MIDLIGHT, MIDLIGHT, 0);
//  } else if (val == 4) {
//    return leds.Color(0, MIDLIGHT, MIDLIGHT);
//  } else if (val == 5) {
//    return leds.Color(MIDLIGHT, 0, MIDLIGHT);
//  } else  {
//    return leds.Color(MIDLIGHT, MIDLIGHT, 0);
//  }
//}
//
//  delay(sensorValue);
//  if (sensorValue > soundLimit) {
//    Serial.print("sensorValue=");
//    Serial.println(sensorValue,  DEC);
//    if (led_msec_val == MAX_DURATION_SEC) {
//      show_msg("Turn on LED", 0);
//      setMyColor();
//      led_msec_val = millis()/1000;
//    } else {
//      show_msg("Turn off LED", 0);
//      turnOffPixel();
//      led_msec_val = MAX_DURATION_SEC;
//    }
//  }
