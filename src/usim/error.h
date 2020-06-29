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

#ifndef ERROR_H
#define ERROR_H

typedef enum ERR_ID
{
    // I/O related error codes.
    ERR_INVALID_ARGUMENT,
    ERR_BIN_READ,
    ERR_NO_DATA,
    ERR_READ_SIZE,
            
    // Memory related error codes.        
    ERR_PROG_MEM_FAIL
            
} ERR_ID;

void printError(ERR_ID errId, char *errData);

#endif /* ERROR_H */

