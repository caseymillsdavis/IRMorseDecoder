/****************************************************************************
 
  Header file for Remote Lock Example Finite Sate Machine 
  based on the Event Framework

 ****************************************************************************/

#ifndef SMmorseDecode_H
#define SMmorseDecode_H

// Event Definitions
#include "EF_Events.h"
#include "misc.h"

// typedefs for the states
// State definitions for use with the query function
// This machine only has one state so what should go in this array?
typedef enum { InitPState, runMorse } TemplateState_t ;


// Public Function Prototypes

boolean InitmorseDecodeSM ( void );
boolean PostmorseDecodeSM( EF_Event ThisEvent );
boolean IsmorseDecodeSMQEmpty( void );
boolean RunmorseDecodeSM( void );
TemplateState_t QuerymorseDecodeSM ( void );


#endif /* SMmorseDecode_H */

