//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Emulator for Arduino platform.
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

#if _PLATFORM != ARDUINO_AVR    
// This file support for Arduino platform(s) only.
#error Unsupported  platform. Support for Arduino platform(s) only.
#endif

#include <Arduino.h>

#include "vmmem84.h"

#define READPORT(p,i,d) ((p & d) | (i & (~d)))

#if defined(ARDUINO_AVR_MEGA2560) 

// PIC and Arduino MEGA 2560 / ATMEGA 2560 Port Mapping:
// 
// ----------|-------------------------------
// PIC Port  |   Arduino / ATMEGA 2560 Port
// ----------|-------------------------------
// PORT A    |   PORT A / PIN A
// TRIS A    |   DDR A
// PORT B    |   PORT C / PIN C
// TRIS B    |   DDR C
// ----------|-------------------------------

#define MAPPING_PORTA PORTA
#define MAPPING_PORTB PORTC

#define MAPPING_DDRA  DDRA
#define MAPPING_DDRB  DDRC

#define MAPPING_PINA  PINA
#define MAPPING_PINB  PINC

#elif defined(ARDUINO_AVR_UNO) 

// PIC and Arduino Uno / ATMEGA 328 Port Mapping:
// 
// ----------|-------------------------------
// PIC Port  |   Arduino / ATMEGA 328 Port
// ----------|-------------------------------
// PORT A    |   PORT C / PIN C
// TRIS A    |   DDR C
// PORT B    |   PORT D / PIN D
// TRIS B    |   DDR D
// ----------|-------------------------------

#define MAPPING_PORTA PORTC
#define MAPPING_PORTB PORTD

#define MAPPING_DDRA  DDRC
#define MAPPING_DDRB  DDRD

#define MAPPING_PINA  PINC
#define MAPPING_PINB  PIND

#endif

void setHardwareRegValue(unsigned char addr, unsigned char val)
{
    switch(addr)
    {
        case ADDR_PORTA:
            MAPPING_PORTA = val;
            break;
        case ADDR_PORTB:
            MAPPING_PORTB = val;
            break;
        case ADDR_TRISA:
            MAPPING_DDRA = ~val;
            break;
        case ADDR_TRISB:
            MAPPING_DDRB = ~val;
            break;    
    }
}

unsigned char getHardwareRegValue(unsigned char addr, unsigned char defaultVal)
{
    switch(addr)
    {
        case ADDR_PORTA:
            return READPORT(MAPPING_PORTA, MAPPING_PINA, MAPPING_DDRA);
        case ADDR_PORTB:
            return READPORT(MAPPING_PORTB, MAPPING_PINB, MAPPING_DDRB);
        case ADDR_TRISA:
            return ~MAPPING_DDRA;
        case ADDR_TRISB:
            return ~MAPPING_DDRB;   
    }

    return defaultVal;
}
