/****************************************************************************
 Module
   morseElementsSM.c

 Description
   This is the Morse elements state machine

****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
*/
#include <ME218_C32.h>
#include <stdio.h>
#include "misc.h"
#include "EF_General.h"
#include "EF_Queue.h"
#include "EF_Events.h"
#include "morseElementsSM.h"
#include "EF_PostList.h"

/*----------------------------- Module Defines ----------------------------*/
#define QUEUE_SIZE 3

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
*/

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
// type of state variable should match htat of enum in header file
static TemplateState_t currentMorseState;
static unsigned int timeOfLastRise;
static unsigned int timeOfLastFall;
static int firstDelta;
static int lengthOfDot;

// everybody needs a queue too!
EF_Event elementsSMQueue[QUEUE_SIZE+1];

//private prototypes
void testCalibration( void);
void characterizeSpace( void);
void characterizePulse( void);


/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
 Function
     InitmorseElementsSM

 Parameters
     None

 Returns
     boolean, False if error in initialization, True otherwise

 Description
     Sets up the event queue and does any rother equired initialization for
     this state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 18:55
****************************************************************************/
boolean InitmorseElementsSM ( void )
{
  unsigned char ucResult1;
  
  EF_Event ThisEvent;
  firstDelta = 0;
  DDRM &= BIT1LO;
  puts("init morse elements sm\r");
  
  ucResult1 = EF_InitQueue( elementsSMQueue, ARRAY_SIZE(elementsSMQueue) );
  if ( ucResult1 ==  QUEUE_SIZE ) // successful queue init
  {
    // put us into the Initial PseudoState
    currentMorseState = initMorseElements;
    // post the initial transition event
    ThisEvent.EventType = EF_INIT;
    if (PostmorseElementsSM( ThisEvent ) == True)
    {
        return True;
    }else
    {
        return False;
    }
  }else
  {
    return False;
  }
}

/****************************************************************************
 Function
     PostmorseElementsSM

 Parameters
     EF_Event ThisEvent , the event to post to the queue

 Returns
     boolean False if the Enqueue operation failed, True otherwise

 Description
     Posts an event to the state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
****************************************************************************/
boolean PostmorseElementsSM( EF_Event ThisEvent )
{
  boolean bResult3;

  bResult3 = EF_EnQueueFIFO( elementsSMQueue, ThisEvent );

  return bResult3;
}

/****************************************************************************
 Function
    RunmorseElementsSM

 Parameters
   void

 Returns
   boolean, Flase if an error encountered, True otherwise

 Description
   add your description here
 Notes
   uses nested switch/case to implement the machine.
 Author
   J. Edward Carryer, 10/23/11, 18:38
****************************************************************************/
boolean RunmorseElementsSM( void )
{
  EF_Event ThisEvent;
  //TemplateState_t nextMorseState;

  //nextMorseState = currentMorseState;
  // pull an event from the queue
  ThisEvent = EF_DeQueue( elementsSMQueue );

  // we should only call the Run function if we know that there is something in
  // the queue, so if we found EF_NO_EVENT something is wrong
  if (ThisEvent.EventType == EF_NO_EVENT )
  {
    return False;
  }else
  {
    switch ( currentMorseState )
    {
        case initMorseElements :       // If current state is initial Psedudo State
            puts("case 1\n\r");
			if ( ThisEvent.EventType == EF_INIT )// only respond to EF_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                currentMorseState = calWaitForRise;
				puts("initMorseElements and next state is calWaitForRise\r");
             }
             break;

        case calWaitForRise : 
			puts("case 2\n\r");		
            switch ( ThisEvent.EventType )
            {
				case calComplete : 
                {
                    currentMorseState = eocWaitRise;
					puts("morse state updated: eocWaitRise\n\r");
                 }
				 break;
				case risingEdge : 
                {
                    timeOfLastRise = ThisEvent.EventParam;
					printf("timeOfLastRise is %d\n", timeOfLastRise);
                    currentMorseState = calWaitForFall;
					puts("calWaitForRise and responded to rising edge\r");
                 } 
                 break;
            }                           
            break;
		case calWaitForFall :
			puts("case 3\n\r");
            switch ( ThisEvent.EventType )
            {
                case fallingEdge : 
                {
                    timeOfLastFall = ThisEvent.EventParam;
					printf("timeOfLastFall is %d\n", timeOfLastFall);
					currentMorseState = calWaitForRise;
					testCalibration();
					puts("calWaitForFall and testCalibration called.\r");
                 }
                 break;
            }                          
            break;	
		case eocWaitRise :
			puts("case 4\n\r");
            switch ( ThisEvent.EventType )
            {
                case risingEdge : 
                {
                    timeOfLastRise = ThisEvent.EventParam;
                    currentMorseState = eocWaitFall;
					characterizeSpace();
                 }
				 break;
				 case buttonDown : 
                {
                    currentMorseState = calWaitForRise;
					firstDelta = 0; 
                 }
                 break;
            }                        
            break;	
		case eocWaitFall :  
			puts("case 5\n\r");
            switch ( ThisEvent.EventType )
            {
                case fallingEdge : 
                {
                    timeOfLastFall = ThisEvent.EventParam;
					currentMorseState = eocWaitRise;
                 }
				 break;
                case buttonDown : 
                { 
					currentMorseState = calWaitForRise;
					firstDelta = 0;
                 }
				 break;
                case eocDetected : 
                { 
					currentMorseState = decodeWaitFall;
                 }
                 break;
            }                          
            break;	
		case decodeWaitRise :       
			puts("case 6\n\r");
            switch ( ThisEvent.EventType )
            {
                case risingEdge : 
                {
                    timeOfLastRise = ThisEvent.EventParam;
                    currentMorseState = decodeWaitFall;
                    characterizeSpace();
                 }
				 break;
                case buttonDown : 
                {
                    currentMorseState = calWaitForRise;
                    firstDelta = 0;
                 }
                 break;
            }
            break;	
		case decodeWaitFall : 
			puts("case 7\n\r");
            switch ( ThisEvent.EventType )
            {
                case fallingEdge : 
                {
                    timeOfLastFall = ThisEvent.EventParam;
					currentMorseState = decodeWaitRise;
					characterizePulse();
                 }
				 break;
                case buttonDown : 
                {
					currentMorseState = calWaitForRise;
					firstDelta = 0;
                 }
				 break;
            }                           
            //currentMorseState = nextMorseState; 
			break;
			
    } 
    return True;
  }
}

 /****************************************************************************
 Function
     IsmorseElementsSMQEmpty

 Parameters
     void

 Returns
     boolean True if the Queue is empty, False if the queue has something in it

 Description
     tests if this state machine's queue is empty
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:28
****************************************************************************/
boolean IsmorseElementsSMQEmpty( void ){

  return EF_IsQueueEmpty( elementsSMQueue );
}

/****************************************************************************
 Function
     QuerymorseElementsSM

 Parameters
     None

 Returns
     TemplateState_t The current state of the Template state machine

 Description
     returns the current state of the Template state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:21
****************************************************************************/
TemplateState_t QuerymorseElementsSM ( void )
{
   return(currentMorseState);
}

/***************************************************************************
 private functions
 ***************************************************************************/

void testCalibration( void){

	int secondDelta;
	
	if (firstDelta == 0){
		firstDelta = timeOfLastFall - timeOfLastRise; 
		printf("new firstDelta is %d\n", firstDelta);
	}
	else{
		secondDelta = timeOfLastFall - timeOfLastRise;
		printf("secondDelta is %d\n", secondDelta);
		if ( ((100 * firstDelta) / secondDelta) <= 34){
			EF_Event ThisEvent;
			ThisEvent.EventType = calComplete;
			EF_PostList00(ThisEvent);
			puts("posted: calComplete\n\r");
			lengthOfDot = firstDelta;
			printf("dot length is %d\n", lengthOfDot);
			}
		else if ( ((100 * firstDelta) /secondDelta) >= 297){
			EF_Event ThisEvent;
			ThisEvent.EventType = calComplete;
			EF_PostList00(ThisEvent);
			puts("posted: calComplete\n\r");
			lengthOfDot = secondDelta;
			printf("dot length is %d\n", lengthOfDot);
			}
		else{
			firstDelta = secondDelta;
			}
		}
	return;
}
	
void characterizeSpace( void){

	int lastInterval;
	lastInterval = timeOfLastRise - timeOfLastFall;
	printf("characterize space %d\n", lastInterval);
	
	if ( (lastInterval <= (lengthOfDot*3+60)) && (lastInterval >= (lengthOfDot*3-60))){
		EF_Event ThisEvent;
		ThisEvent.EventType = eocDetected;
		EF_PostList03(ThisEvent);
		puts("eoc detected");
		}
	else{
		if ( (lastInterval <= (lengthOfDot*7+140)) && (lastInterval >= (lengthOfDot*7-140))){
			EF_Event ThisEvent;
		    ThisEvent.EventType = eowDetected;
		    EF_PostList03(ThisEvent);
		    puts("eow detected");
		    }
		else{
			EF_Event ThisEvent;
			ThisEvent.EventType = badSpace;
			EF_PostList03(ThisEvent);
			puts("bad space");
			}
		}
	return;
}

void characterizePulse( void){
	
	int lastPulseWidth;
	lastPulseWidth = timeOfLastFall - timeOfLastRise;
	printf("characterize pulse %d\n", lastPulseWidth);
	
	if( (lastPulseWidth <= (lengthOfDot+20)) && (lastPulseWidth >= (lengthOfDot-20))){
		EF_Event ThisEvent;
		ThisEvent.EventType = dotDetected;
		EF_PostList02(ThisEvent);
		}
	else{
		if ( (lastPulseWidth <= (lengthOfDot*3+60)) && (lastPulseWidth >= (lengthOfDot*3-60))){
			EF_Event ThisEvent;
			ThisEvent.EventType = dashDetected;
			EF_PostList02(ThisEvent);
			}
		else{
			EF_Event ThisEvent;
			ThisEvent.EventType = badSpace;
			EF_PostList03(ThisEvent);
			puts("bad space");
			}
		}
	return;
}