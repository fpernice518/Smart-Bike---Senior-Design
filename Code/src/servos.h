/**
 * @file   servos.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for servo control. <br>
 * @defgroup servos Servos
 * @{
 *
 * This header file contains various function prototypes needed for other 
 * files in this library. These functions communicate to the servo controller
 * via UART and give direction to each servo.  There is also a power control
 * provided by the low side driver MOSFETs.  Switching the gate of the MOSFETs
 * to a logic 1 activates the servos and they will move to the position given
 * by the servo controller.  The MOSFETs' gates are connected to PORTA 0 (Front)
 * & 2 (Rear).  The servo controller is connected to 5v, 6v battery voltage for 
 * the servos and to the UART pins on PORTE 0 (RX) & 1 (Reset).
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef SERVOS_H
#define SERVOS_H
/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
/** A function used to set the current_gear value to the one stored from EEprom
 *  along with resetting the servo controller and setting both servos off 
 *  initially.
 *
 * @par Parameters
 *				-@a front = front gear value to set current front gear to.
 *				-@a rear = rear gear value to set current rear gear to.
 */
void InitServos(uint8_t front, uint8_t rear);

/** A function which takes in a rear gear value and shifts to that value
 *  one gear at a time.  The function uses two different arrays, one for 
 *  up shifting and another for down shifting.  We decide our direction, 
 *  then chose the appropriate array and shift.
 *
 *	@par Parameters
 *  			-@a gear = Rear gear value to shift into.
 *  			
 */
void SetRearGear(uint8_t gear);

/** A function that takes the requested front gear value and shifts to it 
 *  one gear at a time.  This function also only engages the front servo for
 *  1.5 seconds and then cuts power once it is in gear to save power along 
 *  with allowing the dérailleur to trim itself instead of staying in a
 *  position that produces contact with the chain.  Once the front servo 
 *  shifts, the rear will readjust back into the gear it was in previously.  
 *  This readjustment was necessary due to the chain jumping gears when the
 *  front gear was shifted. It is minimized in rear gear 3 and 5 but still 
 *  occurs.  We use our direction to chose between two different arrays for
 *  the servo placement.  When readjusting the rear gears, the array direction
 *  is opposite of the front array's direction.
 *  
 * @par Parameters
 *  			-@a gear = Front gear value to shift into.
 */
void SetFrontGear(uint8_t gear);

/** A function used to immediately disable both rear and front servos by setting
 *  the low side driver MOSFET's gate to 0. 
 */
void killServos();

/** A function used to automatically change the gearing from its current
 *  position into gear one in the front and six in the rear.  
 *
 *	This function is meant to be able to shift into our hill climb gearing
 *  from any gear.  Also it will only shift the front once our gears in the
 *  rear are in the proper transition gears.  
 *  i.e.
 *  Shifting from 3,1 (front, rear) to our hill climb gear, this is worst case.
 *	Shift in the rear from 1 to 3, then shift the front gear down into 2 (2,3).
 *	Shift in the rear from 3 to 5, then shift the front gear down into 1 (1,5).
 *  Lastly shift the rear from 5 into 6 completing the motion.
 */
void HillShift();

/** A function used for general shifting in the front and rear.
 *  Based off the function used for HillShift, except this is multi-directional
 *  and allows for input of gears to shift to. 
 *
 *  As stated before, this function works the same way as HillShift above, the 
 *  front gear will only shift once the rear is in either 3 (if shifting between
 *  2 and 3 in the front) or 5 (if shifting in-between 1 and 2 in the front).  
 *  After the front shift, the rear will make its way one gear at a time to its
 *  final destination.  In this functions use, the front gear doesn't move more 
 *  than one gear at a time, but it contains protections in case it were to 
 *  happen.
 *
 * @par Parameters
 *  			-@a front_gear = front gear value to shift into.
 *				-@a rear_gear = rear gear value to shift into.
 */
void AutomaticShift(uint8_t front_gear, uint8_t rear_gear);

#endif /* SERVOS_H */
/** @} */ /* servos */