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

#ifdef linux
#include <termios.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>

char readKey()
{
    char returnKey = 0;
	
#ifdef linux
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
#endif

#ifdef _WIN32
	DWORD termMode, charCount;
	
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	if(handle == NULL)
	{
		return returnKey;
	}
	
	GetConsoleMode(handle, &termMode);
	SetConsoleMode(handle, (termMode & (~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT))));
	
	if(!ReadConsole(handle, &returnKey, 1, &charCount, NULL))
	{
		return returnKey;
	}
	
	SetConsoleMode(handle, termMode);	
#endif
        
    return returnKey;
}