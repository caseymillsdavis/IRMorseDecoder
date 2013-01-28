/****************************************************************************
 Module
   morseDecodeSM.c

 Description
   This is the morse decode state machine.

****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
*/
#include <string.h>
#include "misc.h"
#include "EF_General.h"
#include "EF_Queue.h"
#include "EF_Events.h"
#include "EF_PostList.h"
#include "morseDecodeSM.h"
#include "LCD.h"

/*----------------------------- Module Defines ----------------------------*/
#define QUEUE_SIZE 3

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
*/

void clearMorse( void);
char decodeMorse( void); 

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
// type of state variable should match htat of enum in header file
static TemplateState_t currentDecodeState;
char legalChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890?.,:'-/()\"= !$&+;@_";
char morseCode[][8] ={ ".-","-...","-.-.","-..",".","..-.","--.",
					  "....","..",".---","-.-",".-..","--","-.","---",
					  ".--.","--.-",".-.","...","-","..-","...-",
					  ".--","-..-","-.--","--..",".----","..---",
					  "...--","....-",".....","-....","--...","---..",
					  "----.","-----","..--..",".-.-.-","--..--",
					  "---...",".----.","-....-","-..-.","-.--.-",
					  "-.--.-",".-..-.","-...-","-.-.--","...-..-",
					  ".-...",".-.-.","-.-.-.",".--.-.","..--.-"
					 };
 char morseString[8];

// everybody needs a queue too!
EF_Event decodeSMQueue[QUEUE_SIZE+1];


/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
 Function
     InitTemplateSM

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
boolean InitmorseDecodeSM ( void )
{
  unsigned char ucResult1;
  EF_Event ThisEvent;
  
  //additional line
  clearMorse();
  puts("morse decode intialized\r");
  
           
  ucResult1 = EF_InitQueue( decodeSMQueue, ARRAY_SIZE(decodeSMQueue) );
  if ( ucResult1 ==  QUEUE_SIZE ) // successful queue init
  {
    // put us into the Initial PseudoState
    currentDecodeState = InitPState;
    // post the initial transition event
    ThisEvent.EventType = EF_INIT;
    if (PostmorseDecodeSM( ThisEvent ) == True)
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
     PostTemplateSM

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
boolean PostmorseDecodeSM( EF_Event ThisEvent )
{
  boolean bResult3;

  bResult3 = EF_EnQueueFIFO( decodeSMQueue, ThisEvent );

  return bResult3;
}

/****************************************************************************
 Function
    RunTemplateSM

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
boolean RunmorseDecodeSM( void )
{
  EF_Event ThisEvent;

  // pull an event from the queue
  ThisEvent = EF_DeQueue( decodeSMQueue );

  // we should only call the Run function if we know that there is something in
  // the queue, so if we found EF_NO_EVENT something is wrong
  if (ThisEvent.EventType == EF_NO_EVENT )
  {
    return False;
  }else
  {
    switch ( currentDecodeState )
    {
        case InitPState :       // If current state is initial Psedudo State
            if ( ThisEvent.EventType == EF_INIT )// only respond to EF_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                currentDecodeState = runMorse;
             }
            break;
		   case runMorse :       
            switch ( ThisEvent.EventType )
            {
                case dotDetected : 
				puts("in dotDetected\n\r");
                {
                  if ( strlen(morseString) <= 7){
						strcat( morseString, ".");
						return True;
						}
					   else{
					   return False;
						}						
                 }
				 break;
				    case dashDetected : 
					puts("in dashDetected\n\r");
                {
                  if ( strlen(morseString) <= 7){
						strcat( morseString, "-");
						return True;
						}
					   else{
						return False;
						}
                 }
					break;
				    case eocDetected : 
					puts("in eocDetected\n\r");
                {
                    LCDputchar(decodeMorse());
					     clearMorse();
					 return True;
                 } 
				 break;
				    case eowDetected : 
					puts("in eowDetected\n\r");
                {
                    LCDputchar(decodeMorse());
      				  LCDputchar(' ');
      				  clearMorse();
      				  return True;
                 }
				break;				 
				    case buttonDown : 
                {
                    clearMorse();
					 return True;
                 } 
                 break;
			}	 
    }
    return True;
  }
}

 /****************************************************************************
 Function
     IsTemplateSMQEmpty

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
boolean IsmorseDecodeSMQEmpty( void ){

  return EF_IsQueueEmpty( decodeSMQueue );
}

/****************************************************************************
 Function
     QueryTemplateSM

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
TemplateState_t QuerymorseDecodeSM ( void )
{
   return(currentDecodeState);
}

/***************************************************************************
 private functions
 ***************************************************************************/

void clearMorse( void){
	morseString[0] = '\0';
}

char decodeMorse( void){
	int i;
	int len = sizeof(morseCode)/sizeof(char);
	for (i=0; i<len-1; i++){
		if (strcmp(morseString,morseCode[i]) == 0){
			return legalChars[i];
			}
		}
	return '#';
}
		


	
	
