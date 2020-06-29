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

#ifndef HEXPROC_H
#define HEXPROC_H

typedef enum HEX_PROC_STATUS
{
    HEX_EMPTY,
    HEX_ERROR,
    HEX_DATA,
    HEX_END,
    HEX_IGNORE
} HEX_PROC_STATUS;

HEX_PROC_STATUS parseHexLine(char *hexStr, long *addr, unsigned short **data, size_t *dataLen);
unsigned char validateChecksum(unsigned char checkVal, char *data);

#endif /* HEXPROC_H */

