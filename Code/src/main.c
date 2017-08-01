/**
 * @file   main.c  <br>
 * @author Frank Pernice, Dylan Dreisch <br>
 * @date   May 2014  <br>
 * @brief  Main c file  <br>
 * @defgroup main Main
 * @{
 *
 * This source file provides the interconnection of our other files in the   
 * library.  It uses all of our functions and contains the algorithm for
 * top level control along with manual and automatic modes.
 *
 * Included in this file is all of the code for the LED located on the 
 * handlebar.  This this is connected to the AtMega128 on PORTE 7.
 *
 */
 
/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include "smart_bike_package.h"

/*----------------------------------------------------------------------------*/
/* MACROS                                                                     */
/*----------------------------------------------------------------------------*/
#define LED_ON()     PORTE |=  (1 << 7);
#define LED_OFF()    PORTE &= ~(1 << 7);

/*----------------------------------------------------------------------------*/
/* Global Data                                                                */
/*----------------------------------------------------------------------------*/
  uint8_t frontGear;
  uint8_t rearGear;
  uint8_t servoPos;
  uint8_t ticks = 0;
  uint8_t prev_ticks = 0;
  int16_t temp;
  bool_t hill = FALSE;
  bool_t warning = FALSE;
  Accel_stats accel;
  extern bool_t shiftFlag;
  /*table of values used to control automatic shifting, each position relates 
    to a speed.*/
  uint8_t rear_gear_table[32] = {6,6,6,6,6,6,5,6,6,5,4,3,3,2,2,3,3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1};
  uint8_t front_gear_table[32] = {1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
  uint8_t shift_index = 0;
  power_stats pStats;
  
/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/
__eeprom static uint8_t frontEE;
__eeprom static uint8_t rearEE;
bool_t manual_mode(bool_t* automatic);
bool_t automatic_mode(bool_t* automatic);
void AutoShift();
void SingleAutoShift();
void FlashLedOff();
uint8_t GetWarning();  

/*----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                  */
/*----------------------------------------------------------------------------*/
/**	The main function used to initialize our control values like: pStats, on,
 *  automatic, front and rear gears.  This will also call our initialization 
 *  functions for each device (buttons, servos, bluetooth, hall effect, 
 *  MPU6050).  The function will then loop while the value of on is true. In
 *  this loop, the pointer automatic will decide what mode we are in. Once 
 *  shutdown is pressed, the servos will be killed and the LEDs will flash to
 *  signify shutdown was activated.
 *
 */
int main()
{
 bool_t on = TRUE; /*set system to on*/
 pStats = ON;  /*set system to on*/
 bool_t automatic = FALSE; /*initialize to manual*/
 //frontEE = 1;  /*used to set the initial EEprom values.*/
 //rearEE = 6;
 frontGear = frontEE;  /*set the gears to the last written EEprom values*/
 rearGear = rearEE; 

 /*initialize all components*/
 InitButtons();
 InitServos(frontGear, rearGear);
 InitBluetooth();  
 InitHallEffect();
 Init_MPU6050();
  
  /*loop through manual or automatic modes until on = false 
                                               (shutdown pressed)*/
  while(on == TRUE)
  {
    if(automatic == FALSE)
    {
      LED_OFF();
      on = manual_mode(&automatic);
    }
    else
    {
      LED_ON();
      on = automatic_mode(&automatic);
    }
  }
  killServos();
  FlashLedOff();
  FlashLedOff();
  __delay_cycles(8000000);
  PORTE &= ~(1 << 7);
  pStats = OFF;
}

/** A function for manual mode control. This will wait for a button input 
 *  and react in one of seven ways: Front Gear Up, Front Gear Down, Rear 
 *  Gear Up, Rear Gear Down, Shutdown, Switch Mode and Hill Incoming.
 *  
 *  @par Parameters
 *				-@a automatic = a pointer used to denote the mode of
 *                              operation. (automatic or manual).
 *
 *  @param [Out] warning = Boolean used to relay to the app when the 
 *						   rider is pealing too slowly.
 *  @param [Out] pStats = Value used to relay to the app when the rider
 *						  is aloud to power down the bicycle.
 *  @param [Out] FrontEE = EEprom storage of the front gear.
 *	@param [Out] RearEE = EEprom storage of the rear gear.
 *
 *  @returns
 *			-Return a boolean value, will control our variable on and
 *			 ultimately is made false by the Shutdown button.
 *
 */
bool_t manual_mode(bool_t* automatic)
  {
    uint8_t button = GetButtonState();
    uint8_t release_check = button;
    
    while(release_check != BUTTONS_RELEASED && release_check != SHUTDOWN &&
          release_check != HILL_NEARBY && release_check != SWITCH_MODE)
      release_check = GetButtonState();
    
    
    if(button == FRONT_GEAR_UP && frontGear != 3)
    {
      if(GetSpeed() == 0 || GetCadence() <= 24) 
      {
        warning = TRUE;
        return TRUE;
      }
      warning = FALSE;
      LED_ON();
      SetFrontGear(++frontGear);
      LED_OFF();
      return TRUE;
    }
    if(button == FRONT_GEAR_DOWN && frontGear != 1)
    {
      if(GetSpeed() == 0 || GetCadence() <= 24) 
       {
        warning = TRUE;
        return TRUE;
      }
      warning = FALSE;
      LED_ON();
      SetFrontGear(--frontGear);
      LED_OFF();
      return TRUE;
    }
     if(button == REAR_GEAR_UP && rearGear != 7)
    {
      if(GetSpeed() == 0 || GetCadence() <= 24) 
      {
        warning = TRUE;
        return TRUE;
      }
      warning = FALSE;
      LED_ON();
      SetRearGear(++rearGear);
      LED_OFF();
      return TRUE;
    }
    if(button == REAR_GEAR_DOWN && rearGear != 1)
    {
      if(GetSpeed() == 0 || GetCadence() <= 24) 
      {
        warning = TRUE;
        return TRUE;
      }
      warning = FALSE;
      LED_ON();
      SetRearGear(--rearGear);
      LED_OFF();
      return TRUE;
    }
    if(button == SHUTDOWN)
    {
      pStats = POWERDOWN;
      frontEE = frontGear;
      rearEE = rearGear;
      return FALSE;
    }
    if(button == SWITCH_MODE)
    {
      *automatic = TRUE;
      while(GetButtonState() != BUTTONS_RELEASED);
      return TRUE;
    }
    if(button == HILL_NEARBY)
    {
      if (hill == FALSE)
        hill = TRUE;
      else
        hill = FALSE;
    
      if (hill == TRUE)
      {
        HillShift();
        frontGear = 1;
        rearGear = 6;
        return TRUE;
      }
    }
    return TRUE;
  }

/** A function for automatic mode control. This will react when a button 
 *  is input in one of three ways: Shutdown, Switch Mode and Hill Incoming.
 *  Once the function determines no buttons were pressed, it runs the 
 *  SingleAutoShift function to determine which gear the rider should be in.
 *
 *  @par Parameters
 *				-@a automatic = a pointer used to denote the mode of
 *                              operation. (automatic or manual).
 *
 *  @param [Out] pStats = Value used to relay to the app when the rider
 *						  is aloud to power down the bicycle.
 *  @param [Out] FrontEE = EEprom storage of the front gear.
 *	@param [Out] RearEE = EEprom storage of the rear gear.
 *
 *  @returns
 *			-Return a boolean value, will control our variable on and
 *			 ultimately is made false by the Shutdown button.
 *
 */
bool_t automatic_mode(bool_t* automatic)
{
  uint8_t button = GetButtonState();
  uint8_t release_check = button;  
  
  while(release_check != BUTTONS_RELEASED && release_check != SHUTDOWN &&
        release_check != HILL_NEARBY && release_check != SWITCH_MODE)
      release_check = GetButtonState();
    
  if(button == HILL_NEARBY)
  {
    if (hill == FALSE)
      hill = TRUE;
    else
      hill = FALSE;
    
    if (hill == TRUE)
    {
      HillShift();
      frontGear = 1;
      rearGear = 6;
      return TRUE;
    }
  }
  if(button == SHUTDOWN)
    {
      pStats = POWERDOWN;
      frontEE = frontGear;
      rearEE = rearGear;
      return FALSE;
    }
  if(button == SWITCH_MODE)
   {
     *automatic = FALSE;
     ticks = 0;
     while(GetButtonState() != BUTTONS_RELEASED);
     return TRUE;
   }
  SingleAutoShift();
  *automatic = TRUE;
  return TRUE;
}

/** The Autoshift function used to control our gearing in automatic mode. 
 *  Gets a tick value and compares this to the current Shift index value.
 *  If the ticks is larger, then the shift index will increment by one. If
 *  the ticks is lower, then the shift index will decrement by one. Once
 *  adjusted, the function will flash the LEDs to inform the rider and 
 *  shift into gear.  Then the code is delayed two seconds in order to 
 *  prevent rapid shifts along with decreasing the amount of shifts to
 *  save on power.
 *
 * @param [Out] warning = Boolean used to relay to the app when the
 * 					   rider is pealing too slowly. 
 * @param [Out] Shift_flag = Boolean used ensure automatic shifting 
 * 					   		 doesn't occur unless new data is calculated.
 * 
 */
void SingleAutoShift()
{
  if(shiftFlag == TRUE)
  {
    if (ticks == 0)
    {
      shift_index = GetTireTicks();
    }
    ticks = GetTireTicks();
    if(GetCadence() <= 48) 
      {
        warning = TRUE;
        return;
      }
    else 
    {
      warning = FALSE;
      if (ticks > shift_index && shift_index <= 21)
      {
        shift_index += 1;
        FlashLedOff();
        AutomaticShift(front_gear_table[shift_index], rear_gear_table[shift_index]);
      }
      else if (ticks < shift_index && shift_index >= 5)
      {
        shift_index -= 1;
        FlashLedOff();
        AutomaticShift(front_gear_table[shift_index], rear_gear_table[shift_index]);
      }
      else 
        shiftFlag = FALSE;
    } 
    prev_ticks = ticks;
    __delay_cycles(32000000);
  }
}
    
/** A function used to flash the LED off and on twice, over the course of
 *  about 1.5 seconds.
 */
void FlashLedOff()
{
  PORTE &= ~(1 << 7);
  __delay_cycles(8000000);
  PORTE |=  (1 << 7);
  __delay_cycles(8000000);
  PORTE &= ~(1 << 7);
  __delay_cycles(8000000);
  PORTE |=  (1 << 7);
  
}

/** A function used to return our current warning state
 *
 *	@returns
 *			-Returns the Boolean warning
 */
uint8_t GetWarning()
{
  return (uint8_t)warning;
}

/** A function used to return our current power state
 *
 *	@returns
 *			-Returns the custom type power_stats pStats 
 */
power_stats GetPowerStats()
{
  return pStats;
}

/** @} */ /* main */