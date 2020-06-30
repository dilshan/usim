#ifndef ARDUINO_TERMINAL_H
#define ARDUINO_TERMINAL_H

#ifdef _DEBUG_LOG
#define debugLog(r) Serial.println(r)
#else
#define debugLog(r)
#endif

#define LOG_START_EMULATOR  "Starting USim Emulator for Arduino Mega 2560"
#define LOG_SD_INIT_FAIL    "Fail to initialize SD card"
#define LOG_FILE_NOT_FOUND  "Boot file not found"

#endif /* ARDUINO_TERMINAL_H */
