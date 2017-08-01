/**
 * @file   uart.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for UART communication. <br>
 * @defgroup uart UART
 * @{
 *
 * This header file contains various function prototypes needed by 
 * files in this library for UART communication. Also included is one
 * typdef with two states, used to differentiate between the bluetooth 
 * module and the servo controller.
 *
 * UART connects to the servo controller on PORTE 0 (RX) and to the bluetooth
 * module on PORTD 2 & 3.
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef UART_H
#define UART_H

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Typedefs                                                                   */
/*----------------------------------------------------------------------------*/
typedef enum {SERVO_CONTROLLER, BLUETOOTH_MODULE} uart;

/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
/** A function to initialize the UART protocol.  This is used to set baud rate,
 *  transmission speed and enable bits in the control registers.
 *
 *	Contains two major blocks. One is for the Bluetooth module while the other 
 *  is for the servo controller.  If Bluetooth is selected as the device, UART0
 *  is initialized to the proper setting.  If the servo controller is selected,
 *  UART1 is configured to the proper settings.  The device and baud rate are 
 *  passed into this function.  
 *  
 *	@par Parameters
 *  			-@a uart_device = selects the device to set-up (Bluetooth or
 *								  servo controller)
 *				-@a ubrr = baud rate to be set in the UART baud rate register
 *
 */
void InitUART(uart uart_device, uint16_t ubrr);

/** A function to transmit a byte over UART.  
 *  
 *	Contains two major blocks. One is for the Bluetooth module while the other 
 *  is for the servo controller.  If Bluetooth is selected as the device, UART0
 *  is loaded with the data and sent.  If the servo controller is selected,
 *  UART1 is loaded with the data and sent.  The function will hold until 
 *  the data register is empty. The device and byte of data are passed into this  
 *  function.
 *  
 *	@par Parameters
 *  			-@a uart_device = selects the device to set-up (Bluetooth or
 *								  servo controller)
 *				-@a data = one byte of data to be sent over UART
 *
 */
void TransmitUART(uart uart_device, uint8_t data);

/** A function to receive a byte over UART.  
 *  
 *	Contains two major blocks. One is for the Bluetooth module while the other 
 *  is for the servo controller.  If Bluetooth is selected as the device, the
 *  function will wait until the receive complete flag is set then return the data. 
 *  If the servo controller is selected, the function will wait until the receive 
 *  data flag is set then return the data. The device is passed into this function 
 *  and the data received is returned.
 *  
 *	@par Parameters
 *  			-@a uart_device = selects the device to set-up (Bluetooth or
 *								  servo controller)
 * @returns
 *			-Returns the byte of data sent from the selected device.
 *
 */
uint8_t ReceiveUART(uart uart_device);


#endif /* UART_H */
/** @} */ /* uart */