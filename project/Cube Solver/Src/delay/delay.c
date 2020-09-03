#include "main.h"
/*ÏµÍ³Ê±ÖÓ*/
#define SYSCLK 168

	 
void delay_us(unsigned int nus)
{		
	unsigned int ticks;
	unsigned int told,tnow,tcnt=0;
	unsigned int reload=SysTick->LOAD;
	ticks=nus*SYSCLK;
	told=SysTick->VAL;
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;
		}  
	};
}


void delay_ms(unsigned short nms)
{
	unsigned int i;
	for(i=0;i<nms;i++) delay_us(1000);
}

