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
#include "errormsg.h"

#define printErrorStr(...) fprintf(stderr, __VA_ARGS__)

#define errorCase(x,y) case x: errMsg = y; break

void printError(ERR_ID errId, char *errData)
{
    char *errMsg = NULL;
    
    // Get error message related to the specified error ID.
    switch(errId)
    {
        errorCase(ERR_INVALID_ARGUMENT, ERR_STR_INVALID_ARGUMENT);  
        errorCase(ERR_BIN_READ, ERR_STR_BIN_READ); 
        errorCase(ERR_NO_DATA, ERR_STR_NO_DATA); 
        errorCase(ERR_PROG_MEM_FAIL, ERR_STR_PROG_MEM_FAIL); 
        errorCase(ERR_READ_SIZE, ERR_STR_READ_SIZE); 
    }
    
    // Print error message.
    if(errMsg != NULL)
    {
        if(errData != NULL)
        {
            // Print error message with additional data.
            printErrorStr("ERROR : %s %s\n", errMsg, errData);
        }
        else
        {
            // Print only the error message.
            printErrorStr("ERROR : %s\n", errMsg);
        }
    }
}