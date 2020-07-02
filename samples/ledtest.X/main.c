#include <xc.h>

#define _XTAL_FREQ 100000

void main(void) 
{
    PORTB = 0x00;
    TRISB = 0x00;
    
    while(1)
    {
        PORTB += 1;
        
        __delay_ms(250);
    }
    
    return;
}
