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

#include <stdio.h>
#include <string.h>

#include "dump.h"
#include "dumpstr.h"
#include "vmcpu.h"
#include "vmmem84.h"
#include "vmcpuinst.h"

#define SEPERATOR_LINE_LENGTH 80

#define printMemEntry4(s1,v1,s2,v2,s3,v3,s4,v4) \
    printf("%s 0x%02hX\t %s 0x%02hX\t %s 0x%02hX\t %s 0x%02hX\n", \
    s1,v1,s2,v2,s3,v3,s4,v4)

#define printMemEntry3(s1,v1,s2,v2,s3,v3) \
    printf("%s 0x%02hX\t %s 0x%02hX\t %s 0x%02hX \n", \
    s1,v1,s2,v2,s3,v3)

#define printInst(i) printf("\n%s\n", i)
#define printInst2(i,d) printf("\n%s 0x%02hX\n", i,d)
#define printInst3(i,d1,d2) printf("\n%s 0x%02hX, 0x%02hX\n", i,d1, d2)

#define STATUS_BIT(x) ((STATUS & x) ? 1 : 0)

#define getD(d) (d==0) ? 0 : 1

VMCPU cpu;
union DATA_MEM dataMem;

void drawSeperator(const char *title)
{
    unsigned char pos, len;
    
    len = (SEPERATOR_LINE_LENGTH - strlen(title))/2;
    for(pos = 0; pos < len; pos++)
    {
        printf("-");
    }
    
    printf("%s", title);
    len += (strlen(title) % 2);
    
    for(pos = 0; pos < len; pos++)
    {
        printf("-");
    }
    
    printf("\n");
}

void dumpCPUReg()
{
    // Dump CPU register values.
    drawSeperator("CPU Registers");
    printMemEntry3(STR_PC, cpu.PC, STR_WREG, cpu.W, STR_STACK, cpu.stackPtr);
    drawSeperator("");
    printf("\n");
    
    // Dump Special function register values.
    drawSeperator("SFR");
    printMemEntry4(STR_INDF, INDF, STR_TMR0, TMR0, STR_PCL, PCL, STR_FSR, FSR);
    printMemEntry4(STR_PORTA, PORTA, STR_PORTB, PORTB, STR_EEDATA, EEDATA, STR_EEADR, EEADR);    
    printMemEntry4(STR_PCLATH, PCLATH, STR_INTCON, INTCON, STR_OPTION_REG, OPTION_REG, STR_TRISA, TRISA);
    printMemEntry3(STR_TRISB, TRISB, STR_EECON1, EECON1, STR_EECON2, EECON2);
    drawSeperator("");
    
    // Dump status register and bit values.
    printf("%s 0x%02hX\t", STR_STATUS, STATUS);
    printf("%s %d\t %s %d\t %s %d\t %s %d\n", \
        STR_STATUS_C, STATUS_BIT(0x01), STR_STATUS_DC, STATUS_BIT(0x02), \
        STR_STATUS_Z, STATUS_BIT(0x04), STR_STATUS_PD, STATUS_BIT(0x08));

    printf("\t\t%s %d\t %s %d\t %s %d\t %s %d\n", \
        STR_STATUS_TO, STATUS_BIT(0x10), STR_STATUS_RP0, STATUS_BIT(0x20), \
        STR_STATUS_RP1, STATUS_BIT(0x40), STR_STATUS_IRP, STATUS_BIT(0x80));
        
    drawSeperator("");
    printf("\n");
}

void dumpSRAM()
{
    unsigned char memPos;
    unsigned char colCount = 0;
    
    // Dump SRAM space in data memory.
    for(memPos = SRAM_BASE_ADDR; memPos < SRAM_END_ADDR; memPos++)
    {
        printf("0x%02hX\t", dataMem.map[memPos]);
        
        if((++colCount) > 7)
        {
            printf("\n");
            colCount = 0;
        }
    }
    
    printf("\n");
}

void dumpInstruction(unsigned short ins)
{
    if(CHK_INSTRUCTION(ins, ADDEF_MASK, ADDWF))
    {
        printInst3(STR_ADDWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, ANDWF_MASK, ANDWF))
    {
        printInst3(STR_ANDWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, CLRF_MASK, CLRF))
    {
        printInst2(STR_CLRF, (ins & 0x7F));
    }
    else if(CHK_INSTRUCTION(ins, CLRW_MASK, CLRW))
    {
        printInst(STR_CLRW);
    }
    else if(CHK_INSTRUCTION(ins, COMF_MASK, COMF))
    {
        printInst3(STR_COMF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, DECF_MASK, DECF))
    {
        printInst3(STR_DECF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, DECFSZ_MASK, DECFSZ))
    {
        printInst3(STR_DECFSZ, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, INCF_MASK, INCF))
    {
        printInst3(STR_INCF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, INCFSZ_MASK, INCFSZ))
    {
        printInst3(STR_INCFSZ, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, IORWF_MASK, IORWF))
    {
        printInst3(STR_IORWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, MOVF_MASK, MOVF))
    {
        printInst3(STR_MOVF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, MOVWF_MASK, MOVWF))
    {
        printInst3(STR_MOVWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, NOP_MASK, NOP))
    {
        printInst(STR_NOP);
    }
    else if(CHK_INSTRUCTION(ins, RLF_MASK, RLF))
    {
        printInst3(STR_RLF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, RRF_MASK, RRF))
    {
        printInst3(STR_RRF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, SUBWF_MASK, SUBWF))
    {
        printInst3(STR_SUBWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, SWAPF_MASK, SWAPF))
    {
        printInst3(STR_SWAPF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, XORWF_MASK, XORWF))
    {
        printInst3(STR_XORWF, (ins & 0x7F), getD((ins & 0x80)));
    }
    else if(CHK_INSTRUCTION(ins, BCF_MASK, BCF))
    {
        printInst3(STR_BCF, (ins & 0x7F), ((ins & 0x0380) >> 7));
    }
    else if(CHK_INSTRUCTION(ins, BSF_MASK, BSF))
    {
        printInst3(STR_BSF, (ins & 0x7F), ((ins & 0x0380) >> 7));
    }
    else if(CHK_INSTRUCTION(ins, BTFSC_MASK, BTFSC))
    {
        printInst3(STR_BTFSC, (ins & 0x7F), ((ins & 0x0380) >> 7));
    }
    else if(CHK_INSTRUCTION(ins, BTFSS_MASK, BTFSS))
    {
        printInst3(STR_BTFSS, (ins & 0x7F), ((ins & 0x0380) >> 7));
    }
    else if(CHK_INSTRUCTION(ins, ADDLW_MASK, ADDLW))
    {
        printInst2(STR_ADDLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, ANDLW_MASK, ANDLW))
    {
        printInst2(STR_ANDLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, CALL_MASK, CALL))
    {
        printInst2(STR_CALL, (ins & 0x7FF));
    }
    else if(CHK_INSTRUCTION(ins, CLRWDT_MASK, CLRWDT))
    {
        printInst(STR_CLRWDT);
    }
    else if(CHK_INSTRUCTION(ins, GOTO_MASK, GOTO))
    {
        printInst2(STR_GOTO, (ins & 0x7FF));
    }
    else if(CHK_INSTRUCTION(ins, IORLW_MASK, IORLW))
    {
        printInst2(STR_IORLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, MOVLW_MASK, MOVLW))
    {
        printInst2(STR_MOVLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, RETFIE_MASK, RETFIE))
    {
        printInst(STR_RETFIE);
    }
    else if(CHK_INSTRUCTION(ins, RETLW_MASK, RETLW))
    {
        printInst2(STR_RETLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, RETURN_MASK, RETURN))
    {
        printInst(STR_RETURN);
    }
    else if(CHK_INSTRUCTION(ins, SLEEP_MASK, SLEEP))
    {
        printInst(STR_SLEEP);
    }
    else if(CHK_INSTRUCTION(ins, SUBLW_MASK, SUBLW))
    {
        printInst2(STR_SUBLW, (ins & 0xFF));
    }
    else if(CHK_INSTRUCTION(ins, XORLW_MASK, XORLW))
    {
        printInst2(STR_XORLW, (ins & 0xFF));
    }
}

void dumpStack()
{
    unsigned char stackPos;
    
    // Dump details of stack pointer.
    drawSeperator("Stack Pointer");
    printf("%s %d\n", STR_STK_INDEX, cpu.stackPtr);    
    drawSeperator("");
    printf("\n");
    
    // Dump stack.
    drawSeperator("Stack");
    for(stackPos = 0; stackPos < STACK_SIZE; stackPos++)
    {
        printf("0x%02hX\t", cpu.STK[stackPos]);
    }
    
    printf("\n");
    drawSeperator("");
    printf("\n");
}

void dumpReg(unsigned char addr)
{
    unsigned char val = dataMem.map[addr];
    
    if((val > 0x1F) && (val < 0x7F))
    {
        // Dump ASCII character and value.
        
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%02hX (%1$c) \t", val);
#pragma GCC diagnostic pop        
    }
    else
    {
        // Dump value of the target register.
        printf("0x%02hX\t", val);
    }
}