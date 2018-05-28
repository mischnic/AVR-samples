#ifndef WDTCR
#define WDTCR WDTCSR
#endif

#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED 7

void setup() {
  pinMode(LED, OUTPUT);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void loop() {
  digitalWrite(LED, HIGH);
  system_sleep();
  digitalWrite(LED, LOW);

  pinMode(LED, INPUT);
  system_sleep();
  pinMode(LED, OUTPUT);
}


void system_sleep() {
  bitClear(ADCSRA, ADEN);  //Disable ADC

  setup_watchdog(WDTO_2S);
  sleep_mode();

  bitSet(ADCSRA, ADEN);  //Enable ADC
}


//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void setup_watchdog(int timerPrescaler) {
  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 0b0111;
  if (timerPrescaler > 7) bb |= (1 << 5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1 << WDRF); //Clear the watch dog reset
  WDTCR |= (1 << WDCE) | (1 << WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {}