
//
// based on www.elegoo.com tutorial
//

// Define Pins
const int BLUE1 = 3;
const int BLUE2 = 5;
const int  RED1 = 6;
const int  RED2 = 9;
const int  GREEN1 = 10;
const int  GREEN2 = 11;

#define delayTime 2000    // fading time between colors
#define delayTime2 100  // fading time between colors

const int MAX_VALUE = 128;

// define variables
int redValue = MAX_VALUE;
int greenValue = 0;
int blueValue = 0;

void setup()
{
  Serial.begin(9600);
  analogWrite(RED1, redValue);
  analogWrite(RED2, redValue);
  analogWrite(GREEN1, greenValue);
  analogWrite(GREEN2, greenValue);
  analogWrite(BLUE1, blueValue);
  analogWrite(BLUE2, blueValue);
  delay(delayTime);
}

// main loop
void loop()
{
  redValue = MAX_VALUE; // choose a value between 1 and 255 to change the color.
  greenValue = 0;
  blueValue = 0;
  // this is unnecessary as we've either turned on RED in SETUP
  // or in the previous loop ... regardless, this turns RED off
  Serial.println("1 red(255,0), greenValue(0, 255)");
  for(int i = 0; i < MAX_VALUE; i += 1) // fades out red bring green full when i=255
  {
    redValue -= 1;
    greenValue += 1;
    // The following was reversed, counting in the wrong directions
    analogWrite(RED1, redValue);
    analogWrite(GREEN1, greenValue);
    analogWrite(RED2, redValue);
    analogWrite(GREEN2, greenValue);
    delay(delayTime2);
  }
  delay(delayTime);

  redValue = 0;
  greenValue = MAX_VALUE;
  blueValue = 0;

  Serial.println("2 green(255,0), blueValue(0,255)");
  for(int i = 0; i < MAX_VALUE; i += 1) // fades out green bring blue full when i=255
  {
    greenValue -= 1;
    blueValue += 1;
    // The following was reversed, counting in the wrong directions
    analogWrite(GREEN1, greenValue);
    analogWrite(BLUE1, blueValue);
    analogWrite(GREEN2, greenValue);
    analogWrite(BLUE2, blueValue);
    delay(delayTime2);
  }
  delay(delayTime);

  redValue = 0;
  greenValue = 0;
  blueValue = MAX_VALUE;

  Serial.println("3 blue(255,0), redValue(0,255)");
  for(int i = 0; i < MAX_VALUE; i += 1) // fades out blue bring red full when i=255
  {
    // The following code has been rearranged to match the other two similar sections
    blueValue -= 1;
    redValue += 1;
    // The following was reversed, counting in the wrong directions
    analogWrite(BLUE1, blueValue);
//    delay(delayTime);
    analogWrite(RED1, redValue);
//    delay(delayTime);
    analogWrite(BLUE2, blueValue);
    analogWrite(RED2, redValue);
    delay(delayTime2);
  }
  delay(delayTime);
}
