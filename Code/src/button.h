/**
 * @file   button.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file needed by Button.c <br>
 * @defgroup button
 * @{
 *
 * This header file contains defines needed by other files along with function
 * prototypes used to receive button inputs.
 *  
 * The buttons are located in sets of two on each handlebar.  They have custom
 * PCBs ziptied and secured in a position to be accessible to the rider while
 * holding on to the grips of the handle bars.  The buttons are connected to 
 * PORTB 0 (Left handle bar top button), 1 (Left handle bar bottom button),
 * 2 (Right handle bar top button), 3 (Right handle bar bottom button).
 *
 *
 *
 *
 *
 *
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef BUTTON_H
#define BUTTON_H

/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define FRONT_GEAR_UP    0x0E
#define FRONT_GEAR_DOWN  0x0D
#define REAR_GEAR_UP     0x0B
#define REAR_GEAR_DOWN   0x07

#define SWITCH_MODE      0x05
#define SHUTDOWN         0x00
#define HILL_NEARBY      0x0A
#define BUTTONS_RELEASED 0x0F


/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/

/** Initializes timer/counter0 to count up to 100us in CTC mode and trigger an 
 *  interrupt when the count reaches the top value set to 200 with a 
 *  prescaler of 8.
 */
void InitButtons();

/** Takes all values in the current buttonStates array and uses a logic AND 
 *  with the value 0x0F (debounced) to store all 0s located at any part in 
 *  the array. Returns value of debounced with 0s meaning a button was 
 *  pressed.  Four bit values are associated with different inputs and are 
 *  shown above in the Defines section.
 *
 * @returns
 *		- returns debounced, the ANDed value of all values in the buttonStates
 * 		  array.
 *
 */  
uint8_t GetButtonState();

#endif /* BUTTON_H */
/** @} */ /* button */