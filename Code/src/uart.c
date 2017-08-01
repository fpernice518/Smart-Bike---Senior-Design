/**
 * @file   uart.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  UART source file  <br>
 * @defgroup uart UART
 * @{
 *
 * This source file provides the functions that manage the UART  
 * communication protocol.  Provides functions for transmit and receive 
 * along with directions in each function for the bluetooth module
 * and the servo controller
 *
 */

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include "uart.h"

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/
void InitUART(uart uart_device, uint16_t ubrr)
{
  if(uart_device == SERVO_CONTROLLER)
  {
    /* Set baud rate */
    UBRR0H = (uint8_t) (ubrr>>8);
    UBRR0L = (uint8_t)  ubrr;
    
    /* Double transmission speed */
    UCSR0A |= (1<<U2X0);
    /* Enable Recieve and Transmit */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02);
    
    /* Asynchronous, no parity, 1-stop bit, 8-bit frame size*/
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0);
  }
  else if(uart_device == BLUETOOTH_MODULE)
  {
    /* Set baud rate */
    UBRR1H = (uint8_t) (ubrr>>8);
    UBRR1L = (uint8_t)  ubrr;
    
    /* Double transmission speed */
    UCSR1A |= (1<<U2X1);
    /* Enable Recieve and Transmit */
    UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(0<<UCSZ12)|(1<<RXCIE1);
    
    /* Asynchronous, no parity, 1-stop bit, 8-bit frame size*/
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10)|(0<<UPM11)|(0<<UPM10)|(0<<USBS1);
    __enable_interrupt();
  }
}

void TransmitUART(uart uart_device, uint8_t data)
{
  if(uart_device == SERVO_CONTROLLER)
  {
    UDR0 = data;
    while(!(UCSR0A & (1<<UDRE0)));
  }
  else if(uart_device == BLUETOOTH_MODULE)
  {
    UDR1 = data;
    while(!(UCSR1A & (1<<UDRE1)));
  }
}

uint8_t ReceiveUART(uart uart_device)
{
  if(uart_device == SERVO_CONTROLLER)
  {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
  }
  else if(uart_device == BLUETOOTH_MODULE)
  {
    while(!(UCSR1A & (1<<RXC1)));
    return UDR1;
  }
  return -1;
}


/** @} */ /* uart */