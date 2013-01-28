/****************************************************************************
 
  Header file for Remote Lock Example Finite Sate Machine 
  based on the Event Framework

 ****************************************************************************/

#ifndef SMmorseElements_H
#define SMmorseElements_H

// Event Definitions
#include "EF_Events.h"
#include "misc.h"

// typedefs for the states
// State definitions for use with the query function
typedef enum { initMorseElements, calWaitForRise, calWaitForFall, 
               eocWaitRise, eocWaitFall,
			   decodeWaitFall, decodeWaitRise } TemplateState_t ;


// Public Function Prototypes

boolean InitmorseElementsSM ( void );
boolean PostmorseElementsSM( EF_Event ThisEvent );
boolean IsmorseElementsSMQEmpty( void );
boolean RunmorseElementsSM( void );
TemplateState_t QuerymorseElementsSM ( void );


#endif /* SMmorseElements_H */

