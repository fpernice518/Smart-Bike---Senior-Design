/**
 * @file   MPU6050_control.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for the MPU6050. <br>
 * @defgroup MPU6050_control MPU6050
 * @{
 *
 * This header file contains various function prototypes and structures 
 * needed for other files in this library. A structure for each type of 
 * raw data received from the MPU in included along with prototypes for
 * initialization and data reception.
 *
 * This device is powered by 5V and communicates through TWI.  It is  
 * attached to the pins PORTD 0 & 1 of the AtMega128.
 *
 *
 *
 *
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef MPU6050_control_H
#define MPU6050_control_H

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* TYPEDEFS                                                                   */
/*----------------------------------------------------------------------------*/
    typedef struct
    {
      int16_t gyro_x_pos;
      int16_t gyro_y_pos;
      int16_t gyro_z_pos;
      
      float accel_x_pos;
      float accel_y_pos;
      float accel_z_pos;
      
      int16_t temperature;

    } MPU_stats;

    typedef struct
    {
      float x_pos;
      float y_pos;
      float z_pos;
    } Accel_stats;
	
     // float gyro_x_rate;
     // float gyro_y_rate;
     // float gyro_z_rate;
	 
/*----------------------------------------------------------------------------*/
/* FUNCTION Prototypes                                                        */
/*----------------------------------------------------------------------------*/
/** Initialization function to set the correct bits in the corresponding 
 *  registers. 
 *  	-Set the sample rate to 1000Hz in the Sample Rate Divider register.
 *		-Set configuration register to enable the digital low pass filter at 5Hz.
 *		-Set I2C master clock speed to 400kH in the I2C control register.
 *		-Set sleep in the power register to 0.
 */
void Init_MPU6050();

/** A function used to get the Accelerometer, Gyroscope and temperature values
 *  from the appropriate registers. This function will read all the data in 
 *  each register and place it in a Structure for easy use.  This function will
 *  also add any offsets into the raw values received in order to make it 
 *  useful.
 *		
 *	@returns
 *			-Returns the MPU_stats structure stats, containing temperature, 
 *			 gyroscope, and accelerometer values.
 * 
 */
MPU_stats GetMPUStats();

/** A function used to get the temperature from the MPU6050's register.
 *  	
 *	@returns
 *			-Returns the temperature value from the MPU6050.
 */
int16_t getTemp();

#endif /* MPU6050_control_H */ 
/** @} */ /* MPU6050_control */
