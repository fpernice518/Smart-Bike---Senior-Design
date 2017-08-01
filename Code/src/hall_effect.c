/**
 * @file   Hall_effect.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Hall Effect sensors source file  <br>
 * @defgroup hall_effect Hall Effect
 * @{
 *
 * This source file provides the functions that manage data that is produced 
 * from both of our Hall Effect sensors.  Functions include: Initialization,
 * interrupts for counting and scanning intervals along with return value
 * functions.
 *
 */
 
/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "hall_effect.h"
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Global Data                                                                */
/*----------------------------------------------------------------------------*/

float TireHallCount = 0;
float PedalHallCount = 0;
float PedalRPM;
float TireRPM;
float cadence;
float speed;
uint8_t tireTicks;
uint8_t pedalTicks;
int countPedal =0;
int countTire = 0;
bool_t shiftFlag = FALSE;


/*----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                  */
/*----------------------------------------------------------------------------*/
void InitHallEffect()
{
    TCCR1B |= ((1<<CS12) | (1<<WGM12)); // CTC mode and 256 pre-scaler
    
    TIFR |= 1<<OCF1A; // Timer/Counter1, Output Compare A Match Flag
    
    TIMSK |= 1 << OCIE1A;  // Timer/Counter1, Output Compare A Match Interrupt Enable
    
    OCR1A = 62500; // Set time for 1 second 
    
    EIMSK |= (1<<INT6); // Turn on INT6 (Tire)
    
    EICRB |= ((1<<ISC61) | (0<<ISC60));  // Set to activate on falling edge
    
    EIMSK |= (1<<INT5); // Turn on INT5 (Pedal)
    
    EICRB |= ((1<<ISC51) | (0<<ISC50)); // Set to activate on falling edge
    
    __enable_interrupt();
}

/* interrupt activated when the Timer/Counter 1 value reaches 62500 or 1 second. 
   Calculates our speed and cadence using this 1 second interval, then resets the
   values to 0 until the next calculation*/
#pragma vector= TIMER1_COMPA_vect
__interrupt void ISR_COMP1A()
{
  speed = countTire*15*0.081439248;  // (.25(ticks/rotation) * 60s* (MPH conversion)
  tireTicks = countTire;
  cadence = countPedal*12;  // .2 * 60s (5 ticks per rotation)
  countPedal = 0;
  countTire = 0;
  shiftFlag = TRUE;  //Set new data ready flag
}

/* interrupt located on PINE6 and is connected to the signal pin of our tire 
   Hall Effect sensor, it uses INT_6 vector to count negative edges.*/
#pragma vector= INT6_vect
__interrupt void ISR_INT6()
{
  countTire++;
}

/* interrupt located on PINE5 and is connected to the signal pin of our pedal 
   Hall Effect sensor, it uses INT_5 vector to count negative edges.*/
#pragma vector= INT5_vect
__interrupt void ISR_INT5()
{
  countPedal++;
}

float GetSpeed()
{
  return speed;
}

uint8_t GetTireTicks()
{
  return tireTicks;
}

float GetCadence()
{
  return cadence;
}
/** @} */ /* hall_effect */