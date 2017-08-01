/**
 * @file   i2c.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  I2C source file (TWI communication)  <br>
 * @defgroup i2c I2C
 * @{
 *
 * This source file provides the functions that manage the I2C/TWI  
 * communication protocol.  Provides functions for Write, Read, Burst 
 * read/write, start, stop, ACK, NACK and status.
 *
 */
 
/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "i2c.h"

/*----------------------------------------------------------------------------*/
/* MACROS                                                                     */
/*----------------------------------------------------------------------------*/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.
#define TRUE          1
#define FALSE         0

/*----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                  */
/*----------------------------------------------------------------------------*/

static void TWIStart()
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

static void TWIStop()
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

static void TWIWrite(uint8_t u8data)
{
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

static uint8_t TWIReadACK()
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

static uint8_t TWIReadNACK()
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

static uint8_t TWIGetStatus()
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}

void TWIInit()
{
    //set SCL to 400kHz
    TWSR = 0x00;
    TWBR = 0x0C;
    //enable TWI
    TWCR = (1<<TWEN);
}

void TWIWriteByte(uint8_t reg, uint8_t data)
{
	TWIStart();
	TWIWrite((MPU6050_I2C_ADDRESS<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT));
	TWIWrite(reg);
	TWIWrite(data);
	TWIStop();
}

void TWIWriteBurst(uint8_t addr, uint8_t reg, uint8_t const* data, uint8_t length)
{
	uint8_t i;
	
	TWIStart();
	TWIWrite((addr<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT));
	TWIWrite(reg);
	
	for(i=0; i < length; ++i)
	{
		TWIWrite(data[i]);
	}
	
	TWIStop();
}

uint8_t TWIReadByte(uint8_t reg)
{
	uint8_t data;
	TWIStart();
	TWIWrite((MPU6050_I2C_ADDRESS<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT));
	TWIWrite(reg);
	TWIStart();
	TWIWrite((MPU6050_I2C_ADDRESS<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT));
	data = TWIReadNACK();
	TWIStop();
	
	return data;
}

void TWIReadBurst(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t length)
{
	uint8_t i;
	
	TWIStart();
	TWIWrite((addr<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT));
	TWIWrite(reg);
	TWIStart();
	TWIWrite((addr<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT));
	
	for(i=0; i < length-1; ++i)
	{
          data[i] = TWIReadACK();
	}
        
        data[length-1] = TWIReadNACK();
	TWIStop();
}
/** @} */ /* i2c */