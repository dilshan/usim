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

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char readKey()
{
    char returnKey = 0;
    struct termios tterm = {0};
    
    fflush(stdout);
    if(tcgetattr(0, &tterm) < 0)
    {
        return returnKey;
    }
    
    // Initialize new terminal settings.
    tterm.c_lflag &= ~ICANON;
    tterm.c_lflag &= ~ECHO;
    tterm.c_cc[VMIN] = 1;
    tterm.c_cc[VTIME] = 0;
    
    if(tcsetattr(0, TCSANOW, &tterm) < 0)
    {
        return returnKey;
    }
    
    if(read(0, &returnKey, 1) < 0)
    {
        return returnKey;
    }
    
    // Restore terminal settings.
    tterm.c_lflag |= ICANON;
    tterm.c_lflag |= ECHO;
    
    if(tcsetattr(0, TCSADRAIN, &tterm) < 0)
    {
        return returnKey;
    }
        
    return returnKey;
}