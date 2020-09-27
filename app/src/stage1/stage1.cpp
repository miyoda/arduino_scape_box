#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_rgb.h"
#include "../outputs/led_array.h"
#include "stage1.h"
#include "morse_button.h"
#include "rfid_identify_keychain.h"


static struct pt pt_stage1_status; 

void setup_stage1() {
  PT_INIT(&pt_stage1_status);

  setup_morse_button();
  setup_rfid_identify_keychain();
}

int schedule_stage1_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(3000);
  led_array_set(0, LOW); // morse_button
  led_array_set(1, LOW); // rfid_identify_keychain
  led_array_set(2, LOW); // TODO: Toc toc??
  led_array_set(3, LOW); // TODO: caja fuerte potenciometro electrico??
  
  for(;;) {
    PT_SLEEP(pt, 1000);
    if (is_morse_button_passed()) {
      led_array_set(0, HIGH);
    }
    if (is_rfid_identify_keychain_passed()) {
      led_array_set(1, HIGH);
    }
    // TODO 2 leds
    if (is_morse_button_passed() && is_rfid_identify_keychain_passed()) { // TODO 2 leds
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE2);
    }
  }
  PT_END(pt);
}

void loop_stage1() {
  loop_morse_button();
  loop_rfid_identify_keychain();

  PT_SCHEDULE(schedule_stage1_status(&pt_stage1_status));
}