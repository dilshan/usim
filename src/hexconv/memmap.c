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

#include <string.h>
#include <stdlib.h>

#include "error.h"

#define DEFAULT_WORD    0x3FFF

void fillMemoryMap(unsigned short *map, size_t size)
{
    for(size_t pos = 0; pos < size; pos++)
    {
        map[pos] = DEFAULT_WORD;
    }
}

unsigned char updateMemoryMap(unsigned short *map, long baseAddr, unsigned short *recordBlk, size_t recordLen)
{
    // Check availability of memory map and record block.
    if((map == NULL) || (recordBlk == NULL))
    {
        printError(ERR_BAD_BUFFER, NULL);
        return EXIT_FAILURE;
    }
    
    if(recordLen > 0)
    {
        unsigned char *byteMap = (unsigned char *)map;
        memcpy((byteMap + baseAddr), recordBlk, recordLen * 2);
    }

    return EXIT_SUCCESS;
}

long getMaxAddress(long addr1, long addr2)
{
    return (addr1 > addr2) ? addr1 : addr2;
}