/**
 * @file   I2C.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for I2C (TWI communication). <br>
 * @defgroup i2c I2C
 * @{
 *
 * This header file contains various function prototypes needed by 
 * files in this library. Among them are prototypes for initialization 
 * and the three variable return functions.
 *
 * TWI is used to communicate to the MPU6050 on PORTD 0 & 1.
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef I2C_H
#define I2C_H

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/
/** Send Start Signal */
static void TWIStart();

/** Send Stop Signal */
static void TWIStop();

/** Write one byte to the TWI data register
 *
 *  @par Parameters
 * 			-@a data = one byte to be stored in the register
 *
 */
static void TWIWrite(uint8_t data);

/** Read an ACK is received
 *
 * @returns
 *		   -Returns the NACK received. 
 */
static uint8_t TWIReadACK();

/** Read a NACK is received
 *  
 * @returns
 *		   -Returns the NACK received. 
 *
 */
static uint8_t TWIReadNACK();

/** Will return the status of the Status Register
 *
 * @returns
 *		   -Returns the TWI status register.
 */
static uint8_t TWIGetStatus();

/** Initialize TWI
 *  	-set SCL to 400kHz in the TWI bit rate register
 *      -Enable TWI in the TWI control register
 */
void TWIInit();

/** Read an amount of bytes starting from a register address until the maximum
 *  amount is reached.  The register that is read is incremented by 1 after each  
 *  read. This is used to read from registers that have an address difference of 
 *  1 all at once.
 *  
 *	The function sends to addr a request to read reg and stores the data in the 
 *  pointer *data.  The register address is incremented by one along with the array 
 *  index and compared to length.  If equal to or greater than the function ends,  
 *  but if less then the algorithm will loop and request to read from the new  
 *  register, storing the data in the next slot in the array.
 *
 * @par Parameters
 *	        -@a addr = address of the device.
 *			-@a reg  = address of the register to be read from.
 *			-@a data = pointer to the array of data that read data is to be stored.
 *					   Array starts with value to be read from reg, then is 
 *					   incremented by one along with reg.
 *			-@a length = maximum amount of registers to be read from.
 *
 * @param [Out] data = The array to hold the data recieved
 */
void TWIReadBurst(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t length);

/** Read one byte from a register
 *
 *
 * @par Parameters
 *		   -@a reg  = address of the register to be overwritten.
 *
 * @par Assumptions
 *		   -The address the byte is sent to is defaulted to the MPU6050 address
 *
 * @returns
 *		   -Returns the byte received from the requested register.
 */
uint8_t TWIReadByte(uint8_t reg);

/** write an amount of bytes starting from a register address until the maximum
 *  amount is reached.  The register that is written to is incremented by 1   
 *  after each write. This is used to write to registers that have an address  
 *  difference of 1 all at once.
 *
 *	The function sends to addr a request to write over reg using the data in the 
 *  pointer *data.  The register address and array index is incremented by one 
 *  and compared to length.  If equal to or greater than the function ends, but if  
 *  less than, the algorithm will loop and request to write over the new register 
 *  from the data in the next slot in the array.
 *  
 * @par Parameters
 *		   -@a addr = address of the device.
 *		   -@a reg  = address of the register to be overwritten.
 *		   -@a data = pointer to the array of data to be written to each register.
 *				      Array starts with value to be written to reg, then is 
 *				      incremented by one along with reg.
 *		   -@a length = maximum amount of registers to be overwritten.
 *
 */
void TWIWriteBurst(uint8_t addr, uint8_t reg, uint8_t const* data, uint8_t length);

/** Write one byte to a register
 *	
 * @par Parameters
 *  	    -@a reg  = address of the register to be overwritten.
 *			-@a data = one byte to overwrite the data in reg.
 *
 * @par Assumptions
 *		   -The address the byte is sent to is defaulted to the MPU6050 address
 */
void TWIWriteByte(uint8_t reg, uint8_t data);


#endif /* I2C_H */
/** @} */ /* i2c */