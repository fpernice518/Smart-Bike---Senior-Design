/**
 * @file   bluetooth.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for the bluetooth module. <br>
 * @defgroup bluetooth Bluetooth
 * @{
 *
 * This header file contains various function prototypes needed for other 
 * files in this library in-order to use the bluetooth module.
 *
 * The bluetooth module communicates over UART and is connected to the 
 * AtMega128 on PORTD 2 & 3.
 *
 *
 *
 *
 *
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Typedefs                                                                   */
/*----------------------------------------------------------------------------*/
typedef enum {OFF = 0, POWERDOWN, ON} power_stats; 

/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
/** A Function used to initialize PORTD and UART for the Bluetooth module.  
 *  Calls the function InitUART and passes through Bluetooth_modlue to select 
 *  the correct initialization.  Also enables interrupts to ensure they are
 *  enabled.
 */
void InitBluetooth(void);

/** A Function used to reset the bluetooth module.  This will send 1 second long
 *  pulses into the GPIO4 pin of the module and sets the device to factory 
 *  settings.  
 */
void ResetBluetooth(void);

#endif /* BLUETOOTH_H */

/** @} */ /* bluetooth */