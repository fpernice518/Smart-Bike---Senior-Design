/**
 * @file   Hall_Effect.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for Hall Effect. <br>
 * @defgroup hall_effect Hall Effect
 * @{
 *
 * This header file contains various function prototypes needed by 
 * files in this library. Among them are prototypes for initialization 
 * and the three variable return functions. 
 *
 * These functions control the operation of the hall effect sensors. 
 * The sensors are located next to the pedals on the frame and also next
 * to the rear tire on the frame.  The magnets are attached to inner gear
 * on the pedals and on the spokes of the rear tires.  The signal pins of
 * the sensor are connected to PORTE 5 (Pedal) and 6 (Tire).
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
#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/
/** Sets all the proper bits to activate timer/counter 1 into 
 *		-CTC mode with a 256 prescaler.
 *      -Top set to 62500 and will trigger an interrupt every 1 second.
 *  Sets two external interrupts (5&6) on PORTE 5&6 to 
 *  	-trigger on a falling edge.
 */
void  InitHallEffect();

/** Returns the current speed value at the time the function is called
 *
 *  @returns
 *			- returns the current value of speed
 */
float GetSpeed();

/** Returns the current cadence value at the time the function is called
 *
 *  @returns
 *			- returns the current value of cadence
 */
float GetCadence();

/** Returns the current tick value at the time the function is called.
 *  Ticks are the amount of times a magnet passes-through the tire hall
 *  effect sensor.  This directly translates to speed.
 *
 *	@returns
 *			- returns the current value of ticks
 */
uint8_t GetTireTicks();


#endif /* HALL_EFFECT_H */
/** @} */ /* hall_effect */