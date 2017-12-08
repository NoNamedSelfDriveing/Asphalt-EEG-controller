
#include <io.h>
#include <delay.h>

void Putch(char data);

void main(void)
{
    unsigned char string[] = "Hello World! \r\n";     
    unsigned char i = 0;
    
    DDRE = 0xfe;                 // PE 0, 1 -> RX, TX Pin
    UCSR0A = 0x0;                
    UCSR0B = 0b00011000;         // Enable TXENn, RXENn Enable
    UCSR0C = 0b00000110;         // No Parity bit, 1 Stop bit, Transmit 8 bit data 
    UBRR0H = 0;
    UBRR0L = 8;      // 115200 bps
         
        
    while(1){               
        while(string[i] != '\0')
            Putch(string[i++]);   
        
        delay_ms(500);
        i = 0;
    }
    
}                                        

void Putch(char data){
    while((UCSR0A & 0x20) == 0x0);
    UDR0 = data;          
}



