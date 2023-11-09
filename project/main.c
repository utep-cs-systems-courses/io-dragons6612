#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"

#define LED_RED BIT0
#define LED_GREEN BIT6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SWITCHES (SW1 | SW2 | SW3 | SW4)

void switch_init() {
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
}

void led_init(){
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void wdt_init(){
  configureClocks();
  enableWDTInterrupts();
}

int main(void){
  switch_init();
  led_init();
  wdt_init();
  buzzer_init();
  buzzer_set_period(10);
  
  or_sr(0x18);
}

static int buttonDown;

void
switch_interrupt_handler(){
  char p1val = P2IN;

  /*update swithc interrupt sense to detect changes from current buttons */
  P2IES |= (p1val & SWITCHES);
  P2IES &= (p1val | ~SWITCHES);

  /* up = red, down = green */
  if (p1val & SW1){
    P1OUT &= ~LED_GREEN;
    buttonDown = 0;
  }//else{
  //P1OUT |= LED_GREEN;
  // buttonDown = 1;
  //}

  if (p1val & SW2){
    P1OUT &= ~LED_GREEN;
    buzzer_set_period(0);
    buttonDown = 0;
  }/*else{
    P1OUT &= ~LED_RED;
    buzzer_set_period(8000);
    buttonDown = 1;
    }*/

  if (p1val & SW3){
    P1OUT |= LED_GREEN;
    buzzer_set_period(4000);
    buttonDown = 0;
  }/*else{
    P1OUT |= LED_RED;
    buzzer_set_period(1000);
    buttonDown = 1;
  }*/

  if (p1val & SW4){
    P1OUT |= LED_GREEN;
    buttonDown = 0;
  }/*else{
    P1OUT |= LED_RED;
    buzzer_set_period(0);
    buttonDown = 1;
    }*/
}

void
__interrupt_vec(PORT2_VECTOR) Port_2()
{
  if(P2IFG & SWITCHES){
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT(){
  static int blink_count = 0;
  switch (blink_count){
  case 6:
    blink_count = 0;
    P1OUT |= LED_RED;
    break;
  default:
    blink_count ++;
    if(!buttonDown){
      P1OUT &= ~LED_RED;
    }
  }
}
