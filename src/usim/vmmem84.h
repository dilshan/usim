//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
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

#ifndef VMMEM_H
#define VMMEM_H

#define BANK0   0x00
#define BANK1   0x80

#define MAX_DATA_MEM    0x8C
#define SRAM_BASE_ADDR  0x0C
#define SRAM_END_ADDR   0x50

#define ADDR_INDF0      0x00
#define ADDR_INDF1      (ADDR_INDF0 | BANK1)

#define ADDR_PCL0       0x02
#define ADDR_PCL1       (ADDR_PCL0 | BANK1)

#define ADDR_STATUS0    0x03
#define ADDR_STATUS1    (ADDR_STATUS0 | BANK1)

#define ADDR_INTCON0    0x0B
#define ADDR_INTCON1    (ADDR_INTCON0 | BANK1)

#define ADDR_PCLATH0    0x0A
#define ADDR_PCLATH1    (ADDR_PCLATH0 | BANK1)

#define ADDR_FSR0    0x04
#define ADDR_FSR1    (ADDR_FSR0 | BANK1)

#define ADDR_PORTA  0x05
#define ADDR_PORTB  0x06
#define ADDR_TRISA  0x85
#define ADDR_TRISB  0x86

union DATA_MEM
{
    struct 
    {
        // BANK 0
        // Indirect address.
        unsigned char INDF0;
        
        unsigned char TMR0;
        unsigned char PCL0;
        unsigned char STATUS0;
        unsigned char FSR0;
        unsigned char GPIO_PORTA;
        unsigned char GPIO_PORTB;
        
        // Reserved cell.
        unsigned char R00;
        
        unsigned char EEDATA;
        unsigned char EEADR;
        unsigned char PCLATH0;
        unsigned char INTCON0;
        
        // SRAM - General purpose registers.
        unsigned char GPRAM[68];
        
        // Reserved block.
        unsigned char R01[48];
                      
        // BANK 1
        // Indirect address.
        unsigned char INDF1;
        
        unsigned char OPTION_REG;
        unsigned char PCL1;
        unsigned char STATUS1;
        unsigned char FSR1;
        unsigned char GPIO_TRISA;
        unsigned char GPIO_TRISB;
        
        // Reserved cell.
        unsigned char R10;
        
        unsigned char EECON1;
        unsigned char EECON2;
        unsigned char PCLATH1;
        unsigned char INTCON1;        
    } sfr;
    
    unsigned char map[MAX_DATA_MEM];
};

extern union DATA_MEM dataMem;

// Reset values of the special function registers.
#define INDF_RESET          0x00
#define TMR0_RESET          0x00
#define PCL_RESET           0x00
#define STATUS_RESET        0x18
#define FSR_RESET           0x00
#define PORTA_RESET         0x00
#define PORTB_RESET         0x00
#define EEDATA_RESET        0x00
#define EEADR_RESET         0x00
#define PCLATH_RESET        0x00
#define INTCON_RESET        0x00
#define OPTION_REG_RESET    0xFF
#define TRISA_RESET         0xFF
#define TRISB_RESET         0xFF
#define EECON1_RESET        0x00
#define EECON2_RESET        0x00

#define SRAM_RESET          0x00

#define FSR         dataMem.sfr.FSR0
#define PCL         dataMem.sfr.PCL0
#define STATUS      dataMem.sfr.STATUS0
#define INTCON      dataMem.sfr.INTCON0
#define PCLATH      dataMem.sfr.PCLATH0
#define INDF        dataMem.sfr.INDF0

#define TMR0        dataMem.sfr.TMR0
#define GPIO_PORTA  dataMem.sfr.GPIO_PORTA
#define GPIO_PORTB  dataMem.sfr.GPIO_PORTB
#define GPIO_TRISA  dataMem.sfr.GPIO_TRISA
#define GPIO_TRISB  dataMem.sfr.GPIO_TRISB
#define EEDATA      dataMem.sfr.EEDATA
#define EEADR       dataMem.sfr.EEADR
#define OPTION_REG  dataMem.sfr.OPTION_REG
#define EECON1      dataMem.sfr.EECON1
#define EECON2      dataMem.sfr.EECON2

#endif /* VMMEM_H */

