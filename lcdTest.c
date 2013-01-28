#include "LCD.h"

void main( void)
{
   char message[18] = "Scrolling text!   ";
   int i;
   
   LCDInit();
   
   while(1)
   {
      for(i=0; i<18; i++)
      {
         LCDputchar(message[i]);
      }
   }
   return;
}