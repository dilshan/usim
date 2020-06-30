#include <Arduino.h>

#include "vmmem84.h"

void setHardwareRegValue(unsigned char addr, unsigned char val)
{
    switch(addr)
    {
        case ADDR_PORTA:
            PORTA = val;
            break;
        case ADDR_PORTB:
            PORTC = val;
            break;
        case ADDR_TRISA:
            DDRA = ~val;
            break;
        case ADDR_TRISB:
            DDRC = ~val;
            break;    
    }
}

unsigned char getHardwareRegValue(unsigned char addr, unsigned char defaultVal)
{
    switch(addr)
    {
        case ADDR_PORTA:
            return PORTA;
        case ADDR_PORTB:
            return PORTC;
        case ADDR_TRISA:
            return ~DDRA;
        case ADDR_TRISB:
            return ~DDRC;   
    }

    return defaultVal;
}
