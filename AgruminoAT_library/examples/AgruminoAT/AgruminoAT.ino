 /*
  AgruminoAT.ino - AT commands interpreter project for Agrumino board using the Agrumino library.
  Created by Antonello Meloni & Hicham Lafhouli.
*/

#include <Agrumino.h>
#include "AgruminoAt.h"

Agrumino agrumino;
AgruminoAt interpreter;
config* st;

void setup() {
  Serial.begin(115200);
  agrumino.setup();
  agrumino.turnBoardOn();
  interpreter.setup_interpreter(agrumino);
  st = interpreter.get_config();
}

void loop() {
  if (!agrumino.isBoardOn()) {
    agrumino.turnBoardOn();
  }

  boolean isButtonPressed =   agrumino.isButtonPressed();

  /*
    boolean isAttachedToUSB =   agrumino.isAttachedToUSB();
    boolean isBatteryCharging = agrumino.isBatteryCharging();
  */

  if (agrumino.isAttachedToUSB()) {

    Serial.println("Press a key within 2 sec. to go to AT interpreter");
    delay(2000);
    if (Serial.available()) {
      interpreter.atInterpreter();
    }

  }

  if (isButtonPressed) {
    agrumino.turnWateringOn();
    delay(2000);
    agrumino.turnWateringOff();
  }

  blinkLed();

  agrumino.turnBoardOff(); // Board off before delay/sleep to save battery :)

  if (st->sleep_mode == 'd') {
    delaySec(interpreter.get_config()->sleep_time_sec); // The ESP8266 stays powered, executes the loop repeatedly
  } else {

    agrumino.deepSleepSec(interpreter.get_config()->sleep_time_sec); // ESP8266 enter in deepSleep and after the selected time starts back from setup() and then loop()
  }
}

/////////////////////
// Utility methods //
/////////////////////

void blinkLed() {
  agrumino.turnLedOn();
  delay(200);
  agrumino.turnLedOff();
}

void delaySec(int sec) {
  delay (sec * 1000);
}




