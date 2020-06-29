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

#ifndef VMCPUINST_H
#define VMCPUINST_H

// Byte oriented register operations.
#define ADDWF           0x0700  
#define ADDEF_MASK      0x3F00

#define ANDWF           0x0500
#define ANDWF_MASK      0x3F00

#define CLRF            0x0180
#define CLRF_MASK       0x3F80

#define CLRW            0x0100
#define CLRW_MASK       0x3F80

#define COMF            0x0900
#define COMF_MASK       0x3F00

#define DECF            0x0300
#define DECF_MASK       0x3F00

#define DECFSZ          0x0B00
#define DECFSZ_MASK     0x3F00

#define INCF            0x0A00
#define INCF_MASK       0x3F00

#define INCFSZ          0x0F00
#define INCFSZ_MASK     0x3F00

#define IORWF           0x0400
#define IORWF_MASK      0x3F00

#define MOVF            0x0800
#define MOVF_MASK       0x3F00

#define MOVWF           0x0080
#define MOVWF_MASK      0x3F80

#define NOP             0x0000
#define NOP_MASK        0x3F9F

#define RLF             0x0D00
#define RLF_MASK        0x3F00

#define RRF             0x0C00
#define RRF_MASK        0x3F00

#define SUBWF           0x0200
#define SUBWF_MASK      0x3F00

#define SWAPF           0x0E00
#define SWAPF_MASK      0x3F00

#define XORWF           0x0600
#define XORWF_MASK      0x3F00

// Bit oriented register operations.
#define BCF             0x1000
#define BCF_MASK        0x3C00

#define BSF             0x1400
#define BSF_MASK        0x3C00

#define BTFSC           0x1800
#define BTFSC_MASK      0x3C00

#define BTFSS           0x1C00
#define BTFSS_MASK      0x3C00

// Literal and control operations.
#define ADDLW           0x3E00
#define ADDLW_MASK      0x3E00

#define ANDLW           0x3900
#define ANDLW_MASK      0x3F00

#define CALL            0x2000
#define CALL_MASK       0x3800

#define CLRWDT          0x0064
#define CLRWDT_MASK     0x3FFF

#define GOTO            0x2800
#define GOTO_MASK       0x3800

#define IORLW           0x3800
#define IORLW_MASK      0x3F00

#define MOVLW           0x3000
#define MOVLW_MASK      0x3C00

#define RETFIE          0x0009
#define RETFIE_MASK     0x3FFF

#define RETLW           0x3400
#define RETLW_MASK      0x3C00

#define RETURN          0x0008
#define RETURN_MASK     0x3FFF

#define SLEEP           0x0063
#define SLEEP_MASK      0x3FFF

#define SUBLW           0x3C00
#define SUBLW_MASK      0x3E00

#define XORLW           0x3A00
#define XORLW_MASK      0x3F00

#endif /* VMCPUINST_H */

