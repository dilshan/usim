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

#ifndef VMCPU_H
#define VMCPU_H

// CPU reset values.
#define PC_START_ADDRESS  0x00
#define W_START_VALUE 0
#define STACK_START_VALUE 0

#define STACK_SIZE  9

typedef struct VMCPU
{
    unsigned short PC;
    unsigned char W;
    unsigned short STK[STACK_SIZE];
    unsigned char stackPtr;
} VMCPU;

extern VMCPU cpu;

#define CHK_INSTRUCTION(wd, m, ins) ((wd & m) == ins)

#ifdef __cplusplus
extern "C" {
#endif

unsigned short fetchInstruction(unsigned short ins);
void resetProcessor();

#ifdef __cplusplus
}
#endif

#endif /* VMCPU_H */

