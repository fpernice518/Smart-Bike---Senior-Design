/**
 * @file   servos.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  source file for servo control<br>
 * @defgroup servos Servos
 * @{
 *
 * This source file provides the functions used to read and write to the  
 * servo controller and control power to the servos.  Contains functions for 
 * changing gears along with speciality functions like HillShift and KillServos.
 *
 */

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include <ina90.h>
#include "uart.h"
#include "servos.h"

/*----------------------------------------------------------------------------*/
/* MACROS                                                                     */
/*----------------------------------------------------------------------------*/
#define UP   TRUE
#define DOWN FALSE
#define FRONT_SERVO_OFF()    PORTA &= ~(1 << 0);
#define FRONT_SERVO_ON()     PORTA |= (1 << 0); __delay_cycles(100);
#define REAR_SERVO_OFF()     PORTA &= ~(1 << 1);
#define REAR_SERVO_ON()      PORTA |= (1 << 1); __delay_cycles(100);




/*----------------------------------------------------------------------------*/
/* Global Data                                                                */
/*----------------------------------------------------------------------------*/
uint16_t front_gears_up[7][3] =  /*1*/{{4*2000,4*1260,4*1000},  
                                 /*2*/ {4*2000,4*1280,4*1010},  
                                 /*3*/ {4*2000,4*1300,4*1020},  
                                 /*4*/ {4*2000,4*1320,4*1040},  
                                 /*5*/ {4*2000,4*1340,4*1060},  
                                 /*6*/ {4*2000,4*1360,4*1080},  
                                 /*7*/ {4*2000,4*1380,4*1090}}; 
                                                    
uint16_t front_gears_down[7][3] =/*1*/{{4*2000,4*1680,4*1250},  
                                 /*2*/ {4*2000,4*1680,4*1250},  
                                 /*3*/ {4*2000,4*1680,4*1250},  
                                 /*4*/ {4*2000,4*1680,4*1250},  
                                 /*5*/ {4*2000,4*1680,4*1250},  
                                 /*6*/ {4*2000,4*1680,4*1250},  
                                 /*7*/ {4*2000,4*1680,4*1250}}; 

uint16_t rear_gears_up[3][7]  = /*1*/{{4*1742,4*1505,4*1464,4*1434,4*1392,4*1350,4*1250},  
                                /*2*/ {4*1742,4*1585,4*1540,4*1500,4*1450,4*1405,4*1270},  
                                /*3*/ {4*1742,4*1630,4*1588,4*1555,4*1495,4*1450,4*1340}}; 
                                                                                      
uint16_t rear_gears_down[3][7] =/*1*/ {{4*1625,4*1575,4*1535,4*1495,4*1448,4*1395,4*1330}, 
                                /*2*/  {4*1710,4*1654,4*1610,4*1568,4*1523,4*1450,4*1330}, 
                                /*3*/  {4*1770,4*1710,4*1673,4*1632,4*1558,4*1490,4*1330}};


uint8_t current_rear_gear;
uint8_t current_front_gear;

/*----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                  */
/*----------------------------------------------------------------------------*/
/**functions used from hall_effect.h*/
extern float getCadence();
extern float getSpeed();


void InitServos(uint8_t front, uint8_t rear)
{
  current_front_gear = front;
  current_rear_gear = rear;

  SERVO_RESET_DDR  |= (1<<SERVO_RESET_PIN);
  SERVO_RESET_PORT &= ~(1<<SERVO_RESET_PIN);
  __delay_cycles(5000);
  SERVO_RESET_PORT |= (1<<SERVO_RESET_PIN);
  
  InitUART(SERVO_CONTROLLER, UBRR_SERVOS);
  DDRA = 0x03;
  FRONT_SERVO_OFF();
  REAR_SERVO_OFF();
}

void SetRearGear(uint8_t gear)
{
  while(gear != current_rear_gear)
  {
    uint16_t* gear_ptr;
    REAR_SERVO_ON();
  
    if((gear - current_rear_gear) > 0)
    {
      gear_ptr = rear_gears_up[current_front_gear-1];
    }
    else if((gear - current_rear_gear) < 0)
    {
      gear_ptr = rear_gears_down[current_front_gear-1];
      current_rear_gear = current_rear_gear - 2;
    }
    TransmitUART(SERVO_CONTROLLER, 0x84);
    TransmitUART(SERVO_CONTROLLER, REAR_SERVO_CHANNEL);
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr[current_rear_gear] & 0x7F));    /* LSB */
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr[current_rear_gear]>>7) & 0x7F); /* MSB */
    current_rear_gear++;
    __delay_cycles(8000000);
    
  }
}

void SetFrontGear(uint8_t gear)
{
  while(gear != current_front_gear)
  {
    FRONT_SERVO_ON();
    uint16_t* gear_ptr;
    bool_t direction;
    
    if((gear - current_front_gear) > 0)
    {
      gear_ptr = front_gears_up[current_rear_gear-1];
      direction = UP;
    }
    else if((gear - current_front_gear) < 0)
    {
      gear_ptr = front_gears_down[current_rear_gear-1];
      current_front_gear = current_front_gear - 2;
      direction = DOWN;
    }
    TransmitUART(SERVO_CONTROLLER, 0x84);
    TransmitUART(SERVO_CONTROLLER, FRONT_SERVO_CHANNEL);
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr [current_front_gear] & 0x7F));    /* LSB */
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr [current_front_gear]>>7) & 0x7F); /* MSB */
    current_front_gear++;
    __delay_cycles(24000000);
    FRONT_SERVO_OFF();
  
    if(direction == DOWN)
    {
      gear_ptr = rear_gears_up[current_front_gear-1];
    }
    else if(direction == UP)
    {
      gear_ptr = rear_gears_down[current_front_gear-1];
    }
    TransmitUART(SERVO_CONTROLLER, 0x84);
    TransmitUART(SERVO_CONTROLLER, REAR_SERVO_CHANNEL);
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr[current_rear_gear-1] & 0x7F));    /* LSB */
    TransmitUART(SERVO_CONTROLLER, (uint8_t) (gear_ptr[current_rear_gear-1]>>7) & 0x7F); /* MSB */
    __delay_cycles(8000000);
  }
}

void killServos()
{
  REAR_SERVO_OFF();
  FRONT_SERVO_OFF();
}

void HillShift()
{
  for (int i = current_front_gear; i != 0; --i)//shift depending on front position
  {
    if (current_front_gear == 3 && i == 2)// shift into 3 (rear prep gear) if were in 3 in the front
      SetRearGear(3);
    else if (current_front_gear == 2 && i == 1)// shift into 5 (rear prep gear) if were in 2 in the front
      SetRearGear(5);
    SetFrontGear(i);// shift the front down after we're in the correct prep gear
  }
  SetRearGear(6);// shift into 6 to complete hill climb gear
}

void AutomaticShift(uint8_t front_gear, uint8_t rear_gear)
{
  int j;
   if((front_gear - current_front_gear) > 0)
    {
      j = 1;
    }
    else if((front_gear - current_front_gear) < 0)
    {
      j = -1;
    }
   
  for (int i = current_front_gear; i != front_gear; i= i + j)//shift depending on front position
  {
    if (current_front_gear == 1 && j == 1)// shift into 3 (rear prep gear) if we're in 1 in the front
      SetRearGear(5);
    else if (current_front_gear == 2 && j == 1)// shift into 3 (rear prep gear) if we're in 2 in the front
      SetRearGear(3);
    else if (current_front_gear == 3 && j == -1)// shift into 3 (rear prep gear) if we're in 3 in the front
      SetRearGear(3);
    else if (current_front_gear == 2 && j == -1)// shift into 5 (rear prep gear) if we're in 2 in the front
      SetRearGear(5);
    __delay_cycles(1000000);
    SetFrontGear(i+j);// shift the front down after we're in the correct prep gear
  }
  SetRearGear(rear_gear);// shift into requested gear
}

/** @} */ /* servos */