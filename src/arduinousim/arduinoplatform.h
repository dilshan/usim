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

#ifndef ARDUINO_PLATFORM_H
#define ARDUINO_PLATFORM_H

#if _PLATFORM != ARDUINO_AVR    
// This file support for Arduino platform(s) only.
#error Unsupported  platform. Support for Arduino platform(s) only.
#else 

#if defined(ARDUINO_AVR_MEGA2560) 
#define SD_CS_PIN   53
#elif defined(ARDUINO_AVR_UNO) 
#define SD_CS_PIN   10
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

void setHardwareRegValue(unsigned char addr, unsigned char val);
unsigned char getHardwareRegValue(unsigned char addr, unsigned char defaultVal);

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_PLATFORM_H */
