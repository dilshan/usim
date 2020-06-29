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

#ifndef ERROR_H
#define ERROR_H

typedef enum ERR_ID
{
    // I/O related error codes.
    ERR_INVALID_ARGUMENT,
    ERR_HEX_READ,
    ERR_BIN_CREATE,
    
    // Hex parsing related error codes.
    ERR_INVALID_HEX,
    ERR_LARGE_BYTE_LENGTH,
    ERR_INVALID_BYTE_COUNT,
    ERR_CHECKSUM,
    
    // Memory related error codes.        
    ERR_BAD_BUFFER        
} ERR_ID;

void printError(ERR_ID errId, char *errData);

#endif /* MAIN_H */

