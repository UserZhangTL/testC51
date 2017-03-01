#include "AllFunc.h"



void noise()
{
		unsigned int i = 0;
	  for(i = 0;i < 1000;i ++)
	  {
			  delay_ms(1);
			  BEEP = ~BEEP;
    }
	  
}