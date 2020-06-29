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

#ifndef DUMP_H
#define DUMP_H

void dumpCPUReg();
void dumpSRAM();
void dumpInstruction(unsigned short ins);
void dumpStack();
void dumpReg(unsigned char addr);

void drawSeperator(const char *title);

#endif /* DUMP_H */

