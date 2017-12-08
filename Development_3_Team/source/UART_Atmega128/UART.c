
#include <io.h>
#include <delay.h>

void Putch(char data);
void TX0_CH(unsigned char data);
void TX0_STR(unsigned char *str);

void main(void)
{
    unsigned char string[] = "Hello World! \r\n";
    unsigned char i = 0;
    char *pStr;
    
    DDRE = 0xfe;
    UCSR0A = 0x0;
    UCSR0B = 0b00011000;
    UCSR0C = 0b00000110;
    UBRR0H = 0;
    UBRR0L = 8;      //115200 bps
         
        
    while(1){               
        while(string[i] != '\0')
            Putch(string[i++]);   
        
        delay_ms(500);
        i = 0;
        //TX0_CH('a');
        //delay_ms(1000);
    }
    
}                                        

void Putch(char data){
    while((UCSR0A & 0x20) == 0x0);
    UDR0 = data;          
    //UCSR0A |= 0x02;
}

void TX0_CH(unsigned char data){
    while(!(UCSR0A & (1 << UDRE0)));
    
    UDR0 = data;
}

void TX0_STR(unsigned char *str){
    while(*str){
        TX0_CH(*str++);
    }
}