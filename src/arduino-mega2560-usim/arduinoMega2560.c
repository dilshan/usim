//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Emulator for Arduino MEGA 2560.
// 
// Copyright 2020 Dilshan R Jayakody.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#include <Arduino.h>

#include "vmmem84.h"

// PIC and Arduino / ATMEGA 2560 Port Mapping:
// 
// ----------|-------------------------------
// PIC Port  |   Arduino / ATMEGA 2560 Port
// ----------|-------------------------------
// PORT A    |   PORT A
// TRIS A    |   DDR A
// PORT B    |   PORT C
// TRIS B    |   DDR C
// ----------|-------------------------------

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
