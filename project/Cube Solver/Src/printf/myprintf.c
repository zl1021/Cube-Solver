#include "myprintf.h"

int fputc(int ch, FILE *stream)
{
    
    while(!(USART1->SR & 0X40));

		USART1->DR=(uint8_t) ch;
	
    return ch;
}

