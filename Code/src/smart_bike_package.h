/**
 * @file   smart_bike_package.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Combinational header file for all headers. <br>
 * @defgroup smart_bike_package Smart Bike
 * @{
 *
 * This header file contains all other headers created along with  
 * all headers necessary for operation.
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef SMART_BIKE_PACKAGE_H
#define SMART_BIKE_PACKAGE_H

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "user_config.h"
#include TARGET_HEADER
#include INTRINSICS_HEADER

#include "bluetooth.h"
#include "button.h"
#include "hall_effect.h"
#include "i2c.h"
#include "MPU6050_control.h"
#include "servos.h"
#include "hall_effect.h"
#include "uart.h"
#include <math.h>

#endif /* SMART_BIKE_PACKAGE_H */
/** @} */ /* smart_bike_package */