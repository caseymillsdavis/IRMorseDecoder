#include <stdio.h>
#include "EF_Framework.h"
#include "EF_Timers.h"

void main( void) {
EF_Return_t ErrorType;

puts("Starting Remote Lock State Machine using the Event Framework\r");
puts(" Press 'u' to unlock or 'l' to locks\n\r");

// Your hardware initialization function calls go here
EF_Timer_Init(EF_Timer_RATE_1MS);

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

};
