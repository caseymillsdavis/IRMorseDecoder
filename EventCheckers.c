// Event Checking functions for use in testing the CheckUserEvents 
// functionality

#include "EF_General.h"
#include "EF_Events.h"
#include "EF_PostList.h"
#include "EventCheckers.h"

// in a real application, this would be an include for all of the state
// machine functions
#include "TemplateSM.h"

boolean Check4Lock(void)
{ 
  static unsigned char LastPinState = 0; 
  unsigned char CurrentPinState;
  boolean ReturnVal = False;
  
  CurrentPinState =  LOCK_PIN;
  // check for pin high and different from last time
  if ( (CurrentPinState != LastPinState) &&  
       (CurrentPinState == LOCK_PIN) )
  {                     // event detected, so post detected event
    EF_Event ThisEvent;
    ThisEvent.EventType = EF_LOCK;
    ThisEvent.EventParam = 1;
    EF_PostList01(ThisEvent); // this could be any SM post function or EF_PostAll
    ReturnVal = True;
  } 
  LastPinState = CurrentPinState;
   
  return ReturnVal;
}

boolean Check4Unlock(void)
{ 
  static unsigned char LastPinState = 0; 
  unsigned char CurrentPinState;
  boolean ReturnVal = False;
  
  CurrentPinState =  UNLOCK_PIN;
  // check for pin high and different from last time
  if ( (CurrentPinState != LastPinState) &&  
       (CurrentPinState == UNLOCK_PIN) )
  {                     // event detected, so post detected event
    EF_Event ThisEvent;
    ThisEvent.EventType = EF_UNLOCK;
    ThisEvent.EventParam = 1;
    EF_PostList01(ThisEvent); // this could be any SM post function or EF_PostAll
    ReturnVal = True;
  }
  LastPinState = CurrentPinState;
   
  return ReturnVal;
}

