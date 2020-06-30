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

#include "vmcpu.h"
#include "vmmem84.h"
#include "vmcpuinst.h"

union DATA_MEM dataMem;
VMCPU cpu;

#define C(v)    STATUS &= 0xFE; STATUS |= v;  dataMem.sfr.STATUS1 = STATUS
#define DC(v)   STATUS &= 0xFD; STATUS |= ((v) << 1);  dataMem.sfr.STATUS1 = STATUS
#define Z(v)    STATUS &= 0xFB; STATUS |= ((v) << 2);  dataMem.sfr.STATUS1 = STATUS

// Get memory offset based on the RP0 value. 
#define BANK  ((dataMem.sfr.STATUS0 & 0x20) ? BANK1 : BANK0)

static inline unsigned char IncStackPointer()
{
    cpu.stackPtr++;
    
    if(cpu.stackPtr >= STACK_SIZE)
    {
        cpu.stackPtr = 0;
    }

    return cpu.stackPtr;
}

static inline unsigned char sfrDirectAddr(unsigned char addr)
{
    if((addr == ADDR_INDF0) || (addr == ADDR_INDF1))   
    {
        // INDF: Indirect addressing mode.
        return FSR;        
    }    
    else
    {
        // Direct addressing mode with SRAM address overlapping.        
        return (((addr + BANK) >= MAX_DATA_MEM) ? addr : (addr | BANK));
    }    
}

static inline unsigned char getSfrValue(unsigned char addr)
{
    return dataMem.map[addr];
}

static inline void setSfrValue(unsigned char addr, unsigned char val)
{
    if(addr == 0)
    {
        // Handle indirect memory mapping by providing the FSR pointer.
        addr = FSR;
    }
    else if((addr == ADDR_PCL0) || (addr == ADDR_PCL1))
    {
        // Map PCL value and PCLATH values into PC.
        dataMem.sfr.PCL1 = PCL = val;       
        cpu.PC = PCL | (PCLATH) << 8;       
    }            
    else if((addr == ADDR_STATUS0) || (addr == ADDR_STATUS1))
    {
        // Map [7..5] bits of the STATUS register.       
        dataMem.sfr.STATUS1 = STATUS = val;
    }    
    else if((addr == ADDR_FSR0) || (addr == ADDR_FSR1))
    {
        // Map FSR register.
        dataMem.sfr.FSR1 = FSR = val;
    }
    else if((addr == ADDR_INTCON0) || (addr == ADDR_INTCON1))
    {
        // Map INTCON register.
        dataMem.sfr.INTCON1 = INTCON = val;
    }
    else if((addr == ADDR_PCLATH0) || (addr == ADDR_PCLATH1))
    {
        // Map PCLATH register.
        dataMem.sfr.PCLATH1 = PCLATH = val;
    }
    else
    {
        dataMem.map[addr] = val;
    }
}

static inline void setPC(unsigned short pc)
{
    cpu.PC = pc;
    dataMem.sfr.PCL1 = PCL = cpu.PC;
    
    //TODO: Handle max PC count.
}

static inline void resetDataMem()
{
    dataMem.sfr.INDF1 = INDF = INDF_RESET;
    dataMem.sfr.PCL1 = PCL = PCL_RESET;
    dataMem.sfr.STATUS1 = STATUS = STATUS_RESET;
    dataMem.sfr.FSR1 = FSR = FSR_RESET;
    dataMem.sfr.PCLATH1 = PCLATH = PCLATH_RESET;
    dataMem.sfr.INTCON1 = INTCON = INTCON_RESET;  
    
    TMR0 = TMR0_RESET;
    
    GPIO_PORTA = PORTA_RESET;
    GPIO_PORTB = PORTB_RESET;
    TRISA = TRISA_RESET;
    TRISB = TRISB_RESET;
    
    EEDATA = EEDATA_RESET;
    EEADR = EEADR_RESET;
    EECON1 = EECON1_RESET;
    EECON2 = EECON2_RESET;
    
    OPTION_REG = OPTION_REG_RESET;
}

void resetProcessor()
{
    unsigned char pos;
    
    cpu.PC = PC_START_ADDRESS;
    cpu.W = W_START_VALUE;
    cpu.stackPtr = 0;
    
    // Reset address of the stack.
    for(pos = 0; pos < STACK_SIZE; pos++)
    {
        cpu.STK[pos] = STACK_START_VALUE;
    }
    
    // Reset values of the special data registers.
    resetDataMem();
    
    // Reset content of SRAM.
    for(pos = SRAM_BASE_ADDR; pos < SRAM_END_ADDR; pos++)
    {
        dataMem.map[pos] = SRAM_RESET;
    }
}

unsigned short fetchInstruction(unsigned short ins)
{        
    unsigned char reg;
    unsigned char memAddr;
    unsigned char tmp;
    unsigned short ltmp;
    
    if(CHK_INSTRUCTION(ins, ADDEF_MASK, ADDWF))
    {
        // Add W and specified register value.
        // Affected to C, DC and Z flags.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        DC((((reg & 0x0F) + (cpu.W & 0x0F)) > 0x0F ? 1 : 0));
        
        ltmp = reg + cpu.W;
        C(((ltmp > 0xFF) ? 1 : 0));
        
        reg = (unsigned char)ltmp;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, ANDWF_MASK, ANDWF))
    {
        // And W with specified register value.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg &= cpu.W;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, CLRF_MASK, CLRF))
    {
        // Clear specified register.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        
        setSfrValue(memAddr, (((memAddr == 0x03) || (memAddr == 0x83)) ? STATUS_RESET : 0x00));
        Z(1);
    }
    else if(CHK_INSTRUCTION(ins, CLRW_MASK, CLRW))
    {
        // Clear W register.
        // Affected to Z flag.
        
        cpu.W = 0x00;
        Z(1);
    }
    else if(CHK_INSTRUCTION(ins, COMF_MASK, COMF))
    {
        // Complement the value of the specified register.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg = ~reg;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, DECF_MASK, DECF))
    {
        // Decrement the value of the specified register.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg--;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, DECFSZ_MASK, DECFSZ))
    {
        // Decrement the value of the specified register and skip if 0.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg--;
        
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
        
        // Skip next instruction if the value is 0.
        if(reg == 0)
        {
            cpu.PC++;
        }        
    }
    else if(CHK_INSTRUCTION(ins, INCF_MASK, INCF))
    {
        // Increment the value of the specified register.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg++;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, INCFSZ_MASK, INCFSZ))
    {
        // Increment the value of the specified register and skip if 0.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg++;
        
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
        
        // Skip next instruction if the value is 0.
        if(reg == 0)
        {
            cpu.PC++;
        }
    }
    else if(CHK_INSTRUCTION(ins, IORWF_MASK, IORWF))
    {
        // Inclusive OR W with specified register value.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg |= cpu.W;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, MOVF_MASK, MOVF))
    {
        // Move value of the selected register into W or to same register.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, MOVWF_MASK, MOVWF))
    {
        // Move value of W to specified register.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        setSfrValue(memAddr, cpu.W);
    }
    else if(CHK_INSTRUCTION(ins, NOP_MASK, NOP))
    {
        // No operation.
        
        __asm__("nop");
    }
    else if(CHK_INSTRUCTION(ins, RLF_MASK, RLF))
    {
        // Rotate value of the selected registry in left through Carry.
        // Affected to C flag.
                       
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        tmp = (STATUS & 0x01) ? 1 : 0;
        C(((reg & 0x80) ? 1 : 0));
        
        reg = (reg << 1) | tmp;

        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, RRF_MASK, RRF))
    {
        // Rotate value of the selected registry in right through Carry. 
        // Affected to C flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        tmp = (STATUS & 0x01) ? 0x80 : 0;
        C(((reg & 0x01) ? 1 : 0));
        
        reg = (reg >> 1) | tmp;
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, SUBWF_MASK, SUBWF))
    {
        // Subtract W with specified register value.
        // Affected to C, DC and Z flags.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
                
        DC((((reg & 0x0F) + (~(cpu.W) & 0x0F) + 1) > 0x0F ? 1 : 0));
        
        ltmp = ((unsigned char)(~(cpu.W))) + 1 + reg;
        C(((ltmp > 0xFF) ? 1 : 0));
        
        reg = (unsigned char)ltmp;          
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }        
    }
    else if(CHK_INSTRUCTION(ins, SWAPF_MASK, SWAPF))
    {
        // Swap nibbles in specified register value.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        tmp = reg;
        reg = ((reg << 4) & 0xF0) | ((tmp >> 4) & 0x0F);
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, XORWF_MASK, XORWF))
    {
        // Exclusive OR W with specified register value.
        // Affected to Z flag.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);
        
        reg ^= cpu.W;
        Z(((reg == 0) ? 1 : 0));
        
        // If d = 0 store result in W, otherwise set value is source register.
        if(ins & 0x80)
        {
            setSfrValue(memAddr, reg);
        }
        else
        {
            cpu.W = reg;
        }
    }
    else if(CHK_INSTRUCTION(ins, BCF_MASK, BCF))
    {
        // Bit clear of specified register.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);       
        tmp = (ins & 0x0380) >> 7;
        
        reg = reg & (~(1 << tmp));
        
        setSfrValue(memAddr, reg);       
    }
    else if(CHK_INSTRUCTION(ins, BSF_MASK, BSF))
    {
        // Bit set of specified register.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr);        
        tmp = (ins & 0x0380) >> 7;
        
        reg = reg | (1 << tmp);
        
        setSfrValue(memAddr, reg); 
    }
    else if(CHK_INSTRUCTION(ins, BTFSC_MASK, BTFSC))
    {
        // Bit test and skip if value is 0.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr); 
        tmp = (ins & 0x0380) >> 7;
        
        reg = reg & (0x1 << tmp);     
        
        // Skip next instruction if the value is 0.
        if(reg == 0)
        {
            cpu.PC++;
        }
    }
    else if(CHK_INSTRUCTION(ins, BTFSS_MASK, BTFSS))
    {
        // Bit test and skip if value is 1.
        
        memAddr = sfrDirectAddr(ins & 0x7F);
        reg = getSfrValue(memAddr); 
        tmp = (ins & 0x0380) >> 7;
        
        reg = reg & (0x1 << tmp);     
        
        // Skip next instruction if the value is 1.
        if(reg != 0)
        {
            cpu.PC++;
        }
    }
    else if(CHK_INSTRUCTION(ins, ADDLW_MASK, ADDLW))
    {
        // Add literal and W values.
        // Affected to C, DC and Z flags.
        
        reg = ins & 0xFF;
        DC((((reg & 0x0F) + (cpu.W & 0x0F)) > 0x0F ? 1 : 0));
        
        ltmp = reg + cpu.W;
        C(((ltmp > 0xFF) ? 1 : 0));

        cpu.W = (unsigned char)ltmp;        
        Z(((cpu.W == 0) ? 1 : 0));
    }
    else if(CHK_INSTRUCTION(ins, ANDLW_MASK, ANDLW))
    {
        // And literal and W values.
        // Affected to Z flag.
        
        reg = ins & 0xFF;
        
        cpu.W &= reg;
        
        Z(((cpu.W == 0) ? 1 : 0));
    }
    else if(CHK_INSTRUCTION(ins, CALL_MASK, CALL))
    {
        // Call subroutine.
        
        // Push return address into stack.
        IncStackPointer();
        cpu.STK[cpu.stackPtr] = cpu.PC + 1;        
        
        // Get address of the subroutine.
        cpu.PC = ins & 0x7FF;   
        cpu.PC |= (PCLATH & 0x18) << 8;
        
        setPC(cpu.PC);    
        return cpu.PC;
    }
    else if(CHK_INSTRUCTION(ins, CLRWDT_MASK, CLRWDT))
    {
        //TODO: Pending
    }
    else if(CHK_INSTRUCTION(ins, GOTO_MASK, GOTO))
    {
        // Unconditional jump to k + PCLATH address.                
        //PC[0..10] <= k
        //PC[11,12] <= PCLATH[3,4]
        
        cpu.PC = ins & 0x7FF;   
        cpu.PC |= (PCLATH & 0x18) << 8;
        
        setPC(cpu.PC);    
        return cpu.PC;
    }
    else if(CHK_INSTRUCTION(ins, IORLW_MASK, IORLW))
    {
        // Inclusive OR literal with W.
        // Affected to Z flag.
        
        reg = ins & 0xFF;
        
        cpu.W |= reg;
        
        Z(((cpu.W == 0) ? 1 : 0));
    }
    else if(CHK_INSTRUCTION(ins, MOVLW_MASK, MOVLW))
    {
        // Move literal to W.
        
        cpu.W = ins & 0xFF;
    }
    else if(CHK_INSTRUCTION(ins, RETFIE_MASK, RETFIE))
    {
        // Return from the interrupt.
        
        // Set GIE (Global Interrupt Enable) bit.
        INTCON |= 0x80;
                       
        setPC(cpu.STK[cpu.stackPtr]);    
        if(cpu.stackPtr > 0)
        {
            cpu.stackPtr--;
        }
        
        return cpu.PC;
    }
    else if(CHK_INSTRUCTION(ins, RETLW_MASK, RETLW))
    {
        // Return with literal in W register.
        
        reg = ins & 0xFF;                                      
        cpu.W = reg;
        
        setPC(cpu.STK[cpu.stackPtr]);    
        if(cpu.stackPtr > 0)
        {
            cpu.stackPtr--;
        }
        
        return cpu.PC;
    }
    else if(CHK_INSTRUCTION(ins, RETURN_MASK, RETURN))
    {
        // Return from subroutine.                       
        setPC(cpu.STK[cpu.stackPtr]);    
        if(cpu.stackPtr > 0)
        {
            cpu.stackPtr--;
        }
        
        return cpu.PC;
    }
    else if(CHK_INSTRUCTION(ins, SLEEP_MASK, SLEEP))
    {
        // TODO: Pending
    }
    else if(CHK_INSTRUCTION(ins, SUBLW_MASK, SUBLW))
    {
        // Subtract literal and W values.
        // Affected to C, DC and Z flags.
        
        reg = ins & 0xFF;        
        
        DC((((reg & 0x0F) + (~(cpu.W) & 0x0F) + 1) > 0x0F ? 1 : 0));
        
        ltmp = ((unsigned char)(~(cpu.W))) + 1 + reg;        
        C(((ltmp > 0xFF) ? 1 : 0));
        
        cpu.W = (unsigned char)ltmp;        
        Z(((cpu.W == 0) ? 1 : 0));
    }
    else if(CHK_INSTRUCTION(ins, XORLW_MASK, XORLW))
    {
        // Exclusive OR literal with W.
        // Affected to Z flag.
        
        reg = ins & 0xFF;
        
        cpu.W ^= reg;
        
        Z(((cpu.W == 0) ? 1 : 0));
    }
    
    // Increment program counter.
    setPC(cpu.PC + 1);    
    return cpu.PC;
}