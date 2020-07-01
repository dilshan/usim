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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hexproc.h"
#include "error.h"

#define LINE_START_CHAR  ':'

HEX_PROC_STATUS parseHexLine(char *hexStr, long *addr, unsigned short **data, size_t *dataLen)
{
    *addr = 0;
    *dataLen = 0;
    
    if(hexStr == NULL)
    {
        // Line string is NULL.
        return HEX_EMPTY;
    }
    
    size_t lineLen = strlen(hexStr);
    if(lineLen < 2)
    {
        // Current string contains not enough data.
        return HEX_EMPTY;
    }
    
    // Check for valid start character.
    if(hexStr[0] != LINE_START_CHAR)
    {
        // Invalid line start character.
        printError(ERR_INVALID_HEX, NULL);
        return HEX_ERROR;
    }
    
    // Get byte count token from the hex string.
    char byteCountStr[3];    
    memcpy(byteCountStr, hexStr + 1, 2);
    byteCountStr[2] = '\0';
    
    long byteCount = strtol(byteCountStr, NULL, 16);
    if(byteCount < 1)
    {
        // No data contains in this hex line.
        return HEX_EMPTY;
    }
    
    if(byteCount > 0xFF)
    {
        // This parser support data byte lengths up to 255.
        printError(ERR_LARGE_BYTE_LENGTH, NULL);
        return HEX_ERROR;
    }
           
    // Decode address token.
    char addrStr[5];
    memcpy(addrStr, hexStr + 3, 4);
    addrStr[4] = '\0';
    
    *addr = strtol(addrStr, NULL, 16);
    
    // Get hex record type token.
    char recTypeStr[3];
    memcpy(recTypeStr, hexStr + 7, 2);
    byteCountStr[2] = '\0';
    
    long recType = strtol(recTypeStr, NULL, 16);
    
    // Check for unsupported record types.
    if(recType >  1)
    {
        // Ignore the current record.
        return HEX_IGNORE;
    }
    
    if(recType == 1)
    {
        // End of hex file records.
        return HEX_END;
    }
    
    // Current record should be data record.
    assert(recType == 0);
    
    if((byteCount % 2) != 0)
    {
        // Invalid byte count. Only expecting bytes to fill word blocks.
        printError(ERR_INVALID_BYTE_COUNT, NULL);
        return HEX_ERROR;
    }
    
    // Create buffer to hold record data.
    *data = (unsigned short *)malloc((byteCount / 2) * sizeof(unsigned short));
    
    // Start reading record data.
    size_t recPos = 0;
    char byteData[3];
    size_t wordCount = 0;
    
    byteCount *= 2;
    
    while(recPos < byteCount)
    {
        // Read high byte.
        memcpy(byteData, hexStr + 9 + recPos, 2);
        byteData[2] = '\0';       
        (*data)[wordCount] = strtol(byteData, NULL, 16);     
        recPos += 2;
        
        // Read low byte.
        memcpy(byteData, hexStr + 9 + recPos, 2);
        byteData[2] = '\0';                 
        (*data)[wordCount] |= (strtol(byteData, NULL, 16) << 8);
        recPos += 2;
        
        wordCount++;
    }
    
    // End of data record.    
    *dataLen = wordCount;
    
    // Get checksum byte.
    char checksumStr[3];
    memcpy(checksumStr, hexStr + 9 + recPos, 2);
    checksumStr[2] = '\0'; 
    
    unsigned char checksum = (unsigned char)strtol(checksumStr, NULL, 16);
    
    if(!validateChecksum(checksum, hexStr))
    {
        // Checksum fail.
        printError(ERR_CHECKSUM, NULL);
        return HEX_ERROR;
    }
    
    return HEX_DATA;
}

unsigned char validateChecksum(unsigned char checkVal, char *data)
{    
    size_t dataLen = strlen(data) - 3;
    unsigned char checksum = 0;
    char byteData[3];
    
    // Get sum of all record bytes.
    size_t pos = 1;
    while(pos < dataLen)
    {
        memcpy(byteData, data + pos, 2);
        byteData[2] = '\0'; 
        
        checksum += (unsigned char)strtol(byteData, NULL, 16);
        pos += 2;        
    }
    
    // Check calculated checksum with specified value.      
    return (checkVal == (unsigned char)(-checksum));
}
