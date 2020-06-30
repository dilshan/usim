#define _DEBUG_LOG

#include <SD.h>

#include "platformsys.h"
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

    Serial.begin(9600);
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
