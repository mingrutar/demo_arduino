
#include "led_matrix.h"

void Pattern::show() {
	int pause = random(1, DELAYTIME1);
	for (int i = 0; i < N_SIZE; i++) {
		lc.setColumn(0, i, *(pdata+i));
		delay(pause);
	}
	delay(DELAYTIME2);
}
void ByRow::show() {
	for(int row = 0;row < N_SIZE;row++) {
		// delay(DELAYTIME2);
		lc.setRow(0,row, B10100000);
		delay(DELAYTIME2);
	}
}
void ByColumn::show() {
	for(int col = 0; col < N_SIZE; col++) {
		// delay(DELAYTIME2);
		lc.setColumn(0, col, B10100000);
		delay(DELAYTIME2);
	}
}
/*
 * LEDMatrix
 */
LEDMatrix::LEDMatrix() {
	clean();

  display_arr[0] = new Pattern(smile);
  display_arr[1] = new Pattern(heart);
	display_arr[2] = new Pattern(big_o);
	display_arr[3] = new Pattern(sad);
	display_arr[4] = new Pattern(look1);
	display_arr[5] = new Pattern(look2);
	display_arr[6] = new Pattern(look3);
	display_arr[7] = new Pattern(look4);
	display_arr[8] = new Pattern(look5);
	display_arr[9] = new Pattern(look6);
	display_arr[10] = new Pattern(look7);
  // display_arr[4] = new ByRow();
  // display_arr[5] = new ByColumn();
  // Serial.println("LEDMatrix::LEDMatrix 2");
  show_random();
}
void LEDMatrix::clean() {
	lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 4);
  /* and clear the display */
  lc.clearDisplay(0);
  // Serial.println("LEDMatrix::LEDMatrix");
	count_down = 0;
}
int LEDMatrix::process(int opt) {
	Serial.print("LEDMatrix::process, opt=");
  Serial.println(opt);
	if (opt == LEDS_SMILE) {
	  lc.clearDisplay(0);
	  delay(DELAYTIME2);
	  count_down = N_LONGBLINK;
	  n_blink = N_SHORTBLINK;
		show_two[0] = 0;
		show_two[1] = 1;
	  display_arr[show_two[0]]->show();
	} else {
		show_random();
	}
}
// void LEDMatrix::updateTime() {
//   if (count_down > 0) {
// 		count_down--;
//   } else {
//     show_random();                  // change pattern
//   }
// }
// void LEDMatrix::show_random() {
//   lc.clearDisplay(0);
//   delay(DELAYTIME2);
//   count_down = 10;
//   display_arr[1]->show();
// 	blog = false;
// }
void LEDMatrix::updateTime() {
  if (count_down > 0) {
		count_down--;
    if (n_blink > 0)
      --n_blink;
    if (n_blink % 2 == 1) {   // turn off all
      lc.clearDisplay(0);
      delay(DELAYTIME2);
    } else {
			int sel = show_two[count_down % 2];
      display_arr[sel]->show();
    }
  } else {
    show_random();                  // change pattern
  }
}
void LEDMatrix::show_random() {
  lc.clearDisplay(0);
  delay(DELAYTIME2);
  count_down = N_SHORTBLINK * 2;
  n_blink = N_SHORTBLINK;
	show_two[0] = random(2, N_DISPLAY);
	show_two[1] = random(2, N_DISPLAY);
  // Serial.print("LEDMatrix::show_random, show_two=");
	// Serial.print(show_two[0]);
	// Serial.print(',');
  // Serial.println(show_two[1]);
  display_arr[show_two[0]]->show();
}
