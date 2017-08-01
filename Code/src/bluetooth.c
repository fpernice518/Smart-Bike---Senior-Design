/**
 * @file   bluetooth.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  source file for bluetooth module control<br>
 * @defgroup bluetooth Bluetooth
 * @{
 *
 * This source file provides the functions used to read and write to the  
 * bluetooth module.  Contains functions for initialization along with reset
 * and an interrupt service routine for input data.
 */

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "bluetooth.h"
#include "user_config.h"
#include "uart.h"
//#include <stdio.h>
#include TARGET_HEADER
#include INTRINSICS_HEADER


/*----------------------------------------------------------------------------*/
/* Bluetooth Commands                                                         */
/*----------------------------------------------------------------------------*/
#define SPEED       's'
#define CADENCE     'c'
#define TEMPERATURE 't'
#define GEARS       'g'
#define WARNING     'w'
#define POWER_STATS 'p'
/*----------------------------------------------------------------------------*/
/* External Functions                                                         */
/*----------------------------------------------------------------------------*/
extern float GetSpeed();
extern float GetCadence();
extern int16_t getTemp(void);
extern uint8_t GetFrontGear();
extern uint8_t GetRearGear();
extern uint8_t GetWarning(void);

/*----------------------------------------------------------------------------*/
/* Datastructures                                                             */
/*----------------------------------------------------------------------------*/
typedef union
{
  float flt;
  byte_t bytes[sizeof(float)];
} float_u;


/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/
void InitBluetooth(void)
{
  /*** Reset Pin Config ***/
  DDRD = 0x20;    /* Set PD5 as output */
  PORTD = 0x20;   /* Set PD5 to logic 1 */
  
  /** USART Init ***/
  InitUART(BLUETOOTH_MODULE, UBBR_BLUETOOTH);
  __enable_interrupt();
}

void ResetBluetooth(void)
{
  /*** Reset Pin Config ***/
  //DDRD = 0x20;    /* Set PD5 as output */
  PORTD = 0x20;   /* Set PD5 to logic 1 */ 

  PORTD ^= 1 << 5 ;
  __delay_cycles(16000000);
  PORTD ^= 1 << 5 ;
  __delay_cycles(16000000);
  PORTD ^= 1 << 5 ;
  __delay_cycles(16000000);
  PORTD ^= 1 << 5 ;
  __delay_cycles(16000000);
}

#pragma vector = USART1_RXC_vect
__interrupt void ISR_USART1_RXC(void)
{
  char cmd = ReceiveUART(BLUETOOTH_MODULE);
  float_u flt;
  int16_t t;
  switch(cmd)
  {
  case SPEED:
    flt.flt = GetSpeed();

    TransmitUART(BLUETOOTH_MODULE, flt.bytes[3]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[2]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[1]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[0]);
    break;
    
  case CADENCE:
    flt.flt = GetCadence();

    TransmitUART(BLUETOOTH_MODULE, flt.bytes[3]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[2]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[1]);
    TransmitUART(BLUETOOTH_MODULE, flt.bytes[0]);
    break; 
	
  case TEMPERATURE:
    t = getTemp();
    TransmitUART(BLUETOOTH_MODULE, (uint8_t)(t&0xFF));
    TransmitUART(BLUETOOTH_MODULE, (uint8_t)(t>>8));
    break;
  
  case GEARS:
    TransmitUART(BLUETOOTH_MODULE, GetFrontGear());
    TransmitUART(BLUETOOTH_MODULE, GetRearGear());
    break;
    
  case WARNING:
    TransmitUART(BLUETOOTH_MODULE, (uint8_t)GetWarning());
    break;
  
  case POWER_STATS:
    TransmitUART(BLUETOOTH_MODULE, (uint8_t)GetPowerStats());
    break;
    
  }
}

/** @} */ /* bluetooth */
