//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Emulator for Arduino platform.
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

#include "buildconfig.h"

#include <SD.h>

#include "arduinoplatform.h"
#include "arduinocommon.h"
#include "terminal.h"

#include "vmcpu.h"
#include "vmmem84.h"

unsigned char bootFlag;
unsigned short inst;
unsigned short addr, nextAddr;
File binFile;

void setup() 
{
    bootFlag = BOOT_FAILURE;

    Serial.begin(SERIAL_BAUD_RATE);
    debugLog(LOG_START_EMULATOR);

    // Start reading SD card to detect startup application.
    pinMode(SD_CS_PIN, OUTPUT);
    if(!SD.begin())
    {
        debugLog(LOG_SD_INIT_FAIL);
        return;
    }

    // Openning startup BIN file.
    binFile = SD.open(BOOT_FILE, FILE_READ);
    if(!binFile)
    {
        debugLog(LOG_FILE_NOT_FOUND);
        return;
    }

    nextAddr = addr = 0x0000;

    // Start CPU execution.
    resetProcessor();

    // System initialization is successful.
    bootFlag = BOOT_SUCCESS;
}

void loop() 
{
    // Check for successful boot.
    if(!bootFlag)
    {
        return;
    }

    inst = binFile.read();
    inst |= (binFile.read() << 8);
    
    // Execute current instruction and get next PC position.
    nextAddr = fetchInstruction(inst);
    if(nextAddr != (addr + 1))
    {
        // Move file pointer to target address.
        addr = nextAddr;
        binFile.seek(addr << 1);
        return;
    }

    // Move to next address.
    addr++;
}
