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

#include "strutil.h"

void extractFileName(char *srcFile, char *fileName)
{        
    if(srcFile != NULL)
    {
        size_t srcSize = strlen(srcFile);
        size_t strPos;
        
        // Looking for file separator.
        for(strPos = (srcSize - 1); strPos > 0;  strPos--)
        {
            if(srcFile[strPos] == FILE_EXTENSION_SEPERATOR)
            {
                // File separator is located.
                break;
            }
        }
        
        if(strPos == 0)
        {
            // File extension is not available.
            strPos = srcSize;
        }
        
        // Strip file extension from given filename.
        strncpy(fileName, srcFile, strPos);
    }
}