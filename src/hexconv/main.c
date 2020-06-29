//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Intel HEX File Converter Utility.
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
#include <string.h>

#include "error.h"
#include "strutil.h"
#include "hexproc.h"
#include "memmap.h"

#define MAX_MEMORY  128     // Support binaries with maximum of 128K.
#define MEM_SIZE    MAX_MEMORY * 512

#define cleanAndExit(x) (returnStatus = x); goto cleanup

int main(int argc, char** argv) 
{
    int returnStatus = EXIT_SUCCESS;
    
    FILE *hexFile = NULL;
    FILE *binFile = NULL;
    
    char *outFile = NULL;
    unsigned short *appBinMap = NULL;
    char *hexLine = NULL;
    size_t len = 0;
    ssize_t readSize;
    size_t maxAddress = 0;
    
    HEX_PROC_STATUS decodeStatus;
    
    if(argc != 2)
    {
        // Command line argument count is wrong.
        printError(ERR_INVALID_ARGUMENT, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Try to load specified hex file from file system.       
    hexFile = fopen(argv[1], "r");
    if(hexFile == NULL)
    {
        // File read error.
        printError(ERR_HEX_READ, argv[1]);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Create output file name.
    outFile = (char *)malloc(strlen(argv[1] + 1));
    extractFileName(argv[1], outFile);
    
    outFile = (char *)realloc(outFile, strlen(outFile) + 6);
    strcat(outFile, ".bin");
   
    // Create output file to store memory map.       
    binFile = fopen(outFile, "wb"); 
    if(outFile == NULL)
    {
        // File create error.
        printError(ERR_BIN_CREATE, outFile);
        cleanAndExit(EXIT_FAILURE);
    }
    
    // Create output memory buffer.
    appBinMap = (unsigned short *)malloc(MEM_SIZE * sizeof(unsigned short));
    if(appBinMap == NULL)
    {
        printError(ERR_BAD_BUFFER, NULL);
        cleanAndExit(EXIT_FAILURE);
    }
    
    fillMemoryMap(appBinMap, MEM_SIZE);
    
    // Process content of the hex file.           
    while((readSize = getline(&hexLine, &len, hexFile)) != -1)
    {
        if(readSize > 0)
        {
            long addr = 0;
            unsigned short *addrData = NULL;
            size_t addrLen = 0;
            
            // Process current hex line.
            decodeStatus = parseHexLine(hexLine, &addr, &addrData, &addrLen);
            
            if(decodeStatus == HEX_DATA)
            {
                // Update memory map with record data.
                updateMemoryMap(appBinMap, addr, addrData, addrLen);
                maxAddress = getMaxAddress((addr + (addrLen * 2)), maxAddress);
            }
            
            // Clear and release allocated buffers.
            if(addrData != NULL)
            {
                free(addrData);
                addrData = NULL;
            }
            
            // Ignore empty and unsupported records.
            if((decodeStatus == HEX_IGNORE) || (decodeStatus == HEX_EMPTY))
            {                
                continue;
            }
            
            // Terminate application on error.
            if(decodeStatus == HEX_ERROR)
            {                
                returnStatus = EXIT_FAILURE;
                break;
            }
            
            // End of hex file is reached.
            if(decodeStatus == HEX_END)
            {
                break;
            }
        }
    }
    
    // Check for any errors.
    if(returnStatus == EXIT_FAILURE)
    {
        goto cleanup;
    }
    
    // Write current memory map to output file.
    if(maxAddress > 0)
    {
        unsigned char *appBinByteMap = (unsigned char *)appBinMap;
        fwrite(appBinByteMap, 1, maxAddress, binFile);
        fflush(binFile);
    }
        
cleanup:
    
    // Release all allocated resources and handles.
    
    if(hexFile != NULL)
    {
        // Close and release hex input file pointer.
        fclose(hexFile);
        hexFile = NULL;
    }
    
    if(binFile != NULL)
    {
        // Close and release output file.
        fclose(binFile);
        binFile = NULL;
    }
    
    if(outFile != NULL)
    {
        // Release output file name buffer.
        free(outFile);
        outFile = NULL;
    }
    
    if(appBinMap != NULL)
    {
        // Release main memory map.
        free(appBinMap);
        appBinMap = NULL;
    }
    
    return returnStatus;
}

