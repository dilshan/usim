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
#include <stdlib.h>

#include "error.h"
#include "vmcpu.h"
#include "vmmem84.h"
#include "dumpstr.h"
#include "dump.h"
#include "console.h"

#define cleanAndExit(x) (returnStatus = x); goto cleanup

int main(int argc, char** argv) 
{
    int returnStatus = EXIT_SUCCESS;
    size_t binSize = 0;
    size_t progMemReadSize = 0;
    unsigned short *progMem = NULL;
    unsigned char *progMemByte = NULL;
    FILE *binFile = NULL;
    unsigned short loadAddr = PC_START_ADDRESS;
    unsigned short currentAddr;
    char stepKey = 0;
    
    if(argc != 2)
    {
        // Command line argument count is wrong.
        printError(ERR_INVALID_ARGUMENT, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    binFile = fopen(argv[1], "rb");
    if(binFile == NULL)
    {
        // File read error.
        printError(ERR_BIN_READ, argv[1]);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Get file size to create input buffer.
    fseek(binFile, 0L, SEEK_END);
    binSize = ftell(binFile);
    
    if(binSize < 1)
    {
        // Binary file contains no data.
        printError(ERR_NO_DATA, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    progMem = (unsigned short *)malloc(binSize);
    if(progMem == NULL)
    {
        // Error creating program memory space.
        printError(ERR_PROG_MEM_FAIL, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Read binary file content and fill program memory.
    rewind(binFile);
    progMemByte = (unsigned char*)progMem;
    progMemReadSize = fread(progMemByte, 1, binSize, binFile);
    if(progMemReadSize != binSize)
    {
        // Mismatch in read byte size and calculated byte size.
        printError(ERR_READ_SIZE, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Start CPU execution.
    resetProcessor();
    
    while(1)
    {        
        // Execute single instruction.
        currentAddr = loadAddr;
        loadAddr = fetchInstruction(progMem[loadAddr]);
        
        // Wait for next user action.
        stepKey = 0;
        
        while(1)
        {
            stepKey = getChr();
            if((stepKey == 'p') || (stepKey == 'P') || (stepKey == 'r') || (stepKey == 'R'))
            {
                // Dump CPU and memory states.
                printf("\n");
                dumpCPUReg();
            }
            else if((stepKey == 'a') || (stepKey == 'A'))
            {
                // Dump current PC address.
                printf("\n%s 0x%02hX\n", STR_CURRENT_ADDESS, currentAddr);
            }
            else if((stepKey == 'm') || (stepKey == 'M'))
            {
                // Dump SRAM content.
                printf("\n");
                dumpSRAM();
            }
            else if((stepKey == 'i') || (stepKey == 'I'))
            {
                // Dump current instruction.          
                dumpInstruction(progMem[currentAddr]);
            }
            else if((stepKey == 'n') || (stepKey == 'N'))
            {
                // Dump next instruction.          
                dumpInstruction(progMem[loadAddr]);
            }
            else if((stepKey == 's') || (stepKey == 'S'))
            {
                // Dump stack.          
                printf("\n");
                dumpStack();
            }
            else if((stepKey == '1'))
            {
                // Dump PORTA and continue.          
                dumpReg(ADDR_PORTA);
                break;
            }
            else if((stepKey == '2'))
            {
                // Dump PORTB and continue.          
                dumpReg(ADDR_PORTB);
                break;
            }
            else
            {
                // Continue to next instruction.
                printf(".");
                break;
            }
        }
    }
           
cleanup:    
    
    // Release all allocated resources and handles.
    
    if(binFile != NULL)
    {
        // Close and release binary file pointer.
        fclose(binFile);
        binFile = NULL;
    }

    if(progMem != NULL)
    {
        // Release main memory map.
        free(progMem);
        progMem = NULL;
    }
    
    return returnStatus;
}

