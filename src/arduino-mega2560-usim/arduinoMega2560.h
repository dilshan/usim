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

#ifndef ARDUINO_MEGA2560_H
#define ARDUINO_MEGA2560_H

// This file support for Arduino MEGA 2560 platform(s) only.
#if _PLATFORM != ARDUINO_MEGA_2560    
#error Unsupported  platform. Support for Arduino MEGA 2560 platform(s) only.
#endif

#define SD_CS_PIN   53

#ifdef __cplusplus
extern "C" {
#endif

void setHardwareRegValue(unsigned char addr, unsigned char val);
unsigned char getHardwareRegValue(unsigned char addr, unsigned char defaultVal);

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_MEGA2560_H */