/**
 * @file   buttons.c  <br>
 * @author Frank Pernice, Dylan Dreisch (Based off Jack Ganssel code) <br>
 * @date   May 2014  <br>
 * @brief  Button Scan functions <br>
 * @defgroup button Button
 * @{
 *
 * This source file provides the functions used in main to initialize 
 * button scanning, scan the buttons and output correct button combinations
 *
 * The following button de-bounce code is a slightly modified version of the
 * one provided by Jack Ganssel here:
 * http://www.ganssle.com/debouncing-pt2.htm
 * 
 *
 */

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
 
#include "common.h"

/* The following button debounce code is a slightly modified version of the
   one provided by Jack Ganssel here:
   http://www.ganssle.com/debouncing-pt2.htm 
*/

/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define MAX_CHECKS 8

/*----------------------------------------------------------------------------*/
/* Global Data                                                                */
/*----------------------------------------------------------------------------*/
uint8_t buttonStates[MAX_CHECKS] = {0x0F,
                                    0x0F,
                                    0x0F,
                                    0x0F,
                                    0x0F,
                                    0x0F,
                                    0x0F,
                                    0x0F}; /* circular queue */
uint8_t index = 0;

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

void InitButtons()
{
  TCCR0 |= (1<<WGM01)  /* CTC mode */
         | (1<<CS01);  /* prescalar of 8 */
  
  TIMSK |= (1<<OCIE0); /* Enable interupt on compare match */
  
  OCR0 = 200;          /* count up to 100us */
  DDRB = ~0x0F;
  PORTB = 0x0F;
  __enable_interrupt();
}

/** Interrupt servic routine that scans PINB every 100us and stores the data 
 *  into an array selected by an incrementing index value.  Index counts up 
 *  to 7 and then loops to overwrite the previous values.
 *
 *  @param [Out] buttonStates[] The array to hold the values of buttons polled
 *			
 */
#pragma vector= TIMER0_COMP_vect
__interrupt void ButtonPollingISR(void)
{
  buttonStates[index] = PINB & 0x0F;
  index = (index + 1) & 0x07; /* increment index and mask to circle queue */
}

uint8_t GetButtonState()
{
  uint8_t i;
  uint8_t debounced = 0x0F;
  
  for(i = 0; i < MAX_CHECKS; ++i) 
    debounced = debounced & buttonStates[i]; /*and all array values with 0x0F to
											   hold all 0s then return*/
  return debounced;
}

/** @} */ /* button */