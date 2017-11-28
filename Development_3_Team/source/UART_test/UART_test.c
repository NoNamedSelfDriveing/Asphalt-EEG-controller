/*
 * UART_test_2.c
 *
 * Created: 2017-11-28 ¿ÀÈÄ 6:59:33
 * Author: dsm2016
 */

#include <io.h>

void Putch(char data);

void main(void)
{
    char string[] = "Hello World! \r\n";
    char *pStr;
    
    UCSR0A = 0x0;
    UCSR0B = 0b00001000;
	UCSR0C = 0b00000110;
	UBRR0H = 0;
	UBRR0L = 103;
	 	
        
	while(1){
        pStr = string;
	    while(*pStr != 0) Putch(*pStr++);    
	}
    
}

void Putch(char data){
	while((UCSR0A & 0x20) == 0x0);
	UDR0 = data;
}