/**
 * @file   user_config.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Header file for user configuration. <br>
 * @defgroup user_config User Config
 * @{
 *
 * This header file contains various defines and typedefs used in other 
 * files in this library. 
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef USER_CONFIG_H
#define USER_CONFIG_H

/*----------------------------------------------------------------------------*/
/* Device Target Headers                                                      */
/*----------------------------------------------------------------------------*/
#define TARGET_HEADER        <iom128.h>
#define INTRINSICS_HEADER    <intrinsics.h>
#define FREQUENCY 16000000 /* 16 MHZ */

#define delay_ms


/*----------------------------------------------------------------------------*/
/* UART                                                                       */
/*----------------------------------------------------------------------------*/

/* Baud Rate Register Values */
#define UBRR_SERVOS 207
#define UBBR_BLUETOOTH 207

/*----------------------------------------------------------------------------*/
/* I2C                                                                        */
/*----------------------------------------------------------------------------*/
#define MPU6050_I2C_ADDRESS 0x68 /* MPU-6050 Address */

/*----------------------------------------------------------------------------*/
/* SERVOS                                                                     */
/*----------------------------------------------------------------------------*/

typedef enum {FRONT_SERVO_CHANNEL = 0, REAR_SERVO_CHANNEL = 1} servoChannel_t; 
#define SERVO_RESET_PORT          PORTE
#define SERVO_RESET_DDR           DDRE
#define SERVO_RESET_PIN           2





#endif /* USER_CONFIG_H */

/** @} */ /* user_config */