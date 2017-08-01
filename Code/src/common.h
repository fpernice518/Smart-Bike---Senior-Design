/**
 * @file   common.h  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Common header file needed by all child headers in this library. <br>
 * @defgroup common Common
 * @{
 *
 * This header file contains various typedefs needed by other child
 * header files in this library. Among them are defines for each 
 * type of variable type.
 *  
 *
 */
 
/* Used to prevent multiple inclusion of the header file */
#ifndef COMMON_H
#define COMMON_H

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "user_config.h"
#include  TARGET_HEADER
#include  INTRINSICS_HEADER

/*----------------------------------------------------------------------------*/
/* TYPEDEFS                                                                   */
/*----------------------------------------------------------------------------*/

typedef unsigned char     uint8_t    /** portable 8-bit unsigned integer */  ;
typedef unsigned char      byte_t    /** portable 8-bit "byte" */            ;
typedef signed char        int8_t    /** portable 8-bit signed integer */    ;
typedef unsigned int     uint16_t    /** portable 16-bit unsigned integer */ ;
typedef signed int        int16_t    /** portable 16-bit signed integer */   ;
typedef unsigned long    uint32_t    /** portable 32-bit unsigned number */  ;
typedef unsigned long     int32_t    /** portable 32-bit signed number */    ;
typedef enum {TRUE, FALSE} bool_t    /** portable  boolean indicator */      ;

#endif /* COMMON_H */
/** @} */ /* common */