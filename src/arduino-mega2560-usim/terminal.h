//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Emulator for Arduino MEGA 2560.
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
