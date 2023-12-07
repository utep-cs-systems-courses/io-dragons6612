#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"

char switch_state_down1, switch_state_down2, switch_state_down3, switch_state_down4,  switch_state_changed; /* effectively boolean */

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update();
}

//where changes happen to both the buzzer and the graphics

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switch_state_down1 = (p2val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  switch_state_down2 = (p2val & SW2) ? 0 : 1;
  switch_state_down3 = (p2val & SW3) ? 0 : 1;
  switch_state_down4 = (p2val & SW4) ? 0 : 1;
  switch_state_changed = 1;
  led_update();
  if(switch_state_down1){
    buzzer_set_period(5555);
    //graphic(1);
  }else if(switch_state_down2){
    buzzer_set_period(7000);
    //graphic(2);
  }else if(switch_state_down3){
    buzzer_set_period(2575);
    //graphic(3);
  }else if(switch_state_down4){
    buzzer_set_period(0);
    //graphic(4);
  }
}
