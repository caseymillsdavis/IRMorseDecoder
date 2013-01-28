// caseyMorse main module (caseyMorseTestMain.c)

#include <stdio.h>
#include "EF_Framework.h"
#include "EF_Timers.h"
#include "LCD.h"
#include "caseyMorseEventCheckers.h"

void main( void) {
EF_Return_t ErrorType;

puts("Starting the Event Framework\r");
puts("Get it on.\n\r");

puts("about to initialize timer\r");
// Your hardware initialization function calls go here
EF_Timer_Init(EF_Timer_RATE_1MS);

//initialize the display module
LCDInit();

puts("about to enter initialize button module\r");
//initialize the button
initButtonModule( );

//initialize morse line
DDRM &= BIT1LO;

// now initialize the state machines and start them running
ErrorType = EF_Initialize();
if ( ErrorType == Success ) {

  ErrorType = EF_Run();

}
//if we got to here, there was an error
switch (ErrorType){
  case FailedPointer:
    puts("Failed on NULL pointer");
    break;
  case FailedInit:
    puts("Failed Initialization");
    break;
 default:
    puts("Other Failure");
    break;
}
for(;;);

}


