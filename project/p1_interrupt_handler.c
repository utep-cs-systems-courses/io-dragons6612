#include <msp430.h>
#include "switches.h"

/* Switch on P1 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

//.arch msp430g2553
//.p2align 1,0
//.text

//.global __interrupt_vec
//.extern P2IFG, SWITCHES

//__interrupt_vec:
//     cmp #0, P2IFG;
//     cmp #0,
//     jmp out;
//out:
// mov 
