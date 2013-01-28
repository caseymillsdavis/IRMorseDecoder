// morse and button event checking functions

#include <ME218_C32.h>
#include <stdio.h>
#include "EF_General.h"
#include "EF_Events.h"
#include "EF_PostList.h"
#include "EF_Timers.h"
#include "caseyMorseEventCheckers.h"

// state machine headers
#include "morseElementsSM.h"
#include "morseDecodeSM.h"

//module level variables
static unsigned char lastButtonState;
static unsigned int timeOfLastSample;

void initButtonModule( void){
	puts("initialize button module\r");
	//initialize the button module by setting PortM3 as an input
	DDRM &= BIT3LO;
	//sample state of button input pin
	lastButtonState = PTM & BIT3HI;
	//get an intial value for timeOfLastSample
	timeOfLastSample = EF_Timer_GetTime();
	printf("%d\n", timeOfLastSample);
	
}

//Event checker for the button - including debounce algorithm
boolean checkButtonEvents(void)
{ 
  #define DEBOUNCE_TIME 100
  unsigned char currentButtonState = PTM & BIT3HI;
  
  boolean ReturnVal = False;
  
  if ( (EF_Timer_GetTime() - timeOfLastSample) > DEBOUNCE_TIME){
	//puts("in debounce if 1\r");
	if (currentButtonState != lastButtonState){
		puts("in debounce if 2\r");
		if (currentButtonState == BIT3HI){
			 EF_Event ThisEvent;
			 ThisEvent.EventType = buttonUp;
			 EF_PostList03(ThisEvent);
			 puts("in debounce if 3\r");
			 }
		else{
			 EF_Event ThisEvent;
			 ThisEvent.EventType = buttonDown;
			 EF_PostList03(ThisEvent);
			 puts("in debounce if-else 3\r");
			 }
		ReturnVal = True;
		}
	timeOfLastSample = EF_Timer_GetTime();	
    lastButtonState = currentButtonState;	
	}
  return ReturnVal;
}

//event checker for the morse input line
boolean checkMorseEvents(void)
{
static unsigned char lastMorseLine = 0x0;
char unsigned currentMorseLine = PTM & BIT1HI;
boolean ReturnVal = False;

if( currentMorseLine != lastMorseLine){
	puts("morse line changed\r");
	if( currentMorseLine == BIT1HI){
		EF_Event ThisEvent;
		ThisEvent.EventType = risingEdge;
		ThisEvent.EventParam = EF_Timer_GetTime();
		EF_PostList00(ThisEvent);
		puts("posted rising edge\r");
		}
	else{
		EF_Event ThisEvent;
		ThisEvent.EventType = fallingEdge;
		ThisEvent.EventParam = EF_Timer_GetTime();
		EF_PostList00(ThisEvent);
		puts("posted falling edge\r");
		}
		
	ReturnVal = True;
	}
lastMorseLine = currentMorseLine;	
return ReturnVal;
}

