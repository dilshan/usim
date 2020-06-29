//------------------------------------------------------------------------------
// USim - Lightweight Microcontroller Simulator.
// Intel HEX File Converter Utility.
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

#ifndef ERRORMSG_H
#define ERRORMSG_H

#define ERR_STR_INVALID_ARGUMENT        "Command line arguments are missing or not supported"
#define ERR_STR_HEX_READ                "Unable to read specified hex file"
#define ERR_STR_BIN_CREATE              "Unable to create output file"
#define ERR_STR_INVALID_HEX             "Unsupported or corrupted hex file"
#define ERR_STR_ERR_LARGE_BYTE_LENGTH   "Data byte length is more than 0xFF"
#define ERR_STR_INVALID_BYTE_COUNT      "Invalid byte count"
#define ERR_STR_CHECKSUM                "Checksum fail"
#define ERR_STR_BAD_BUFFER              "Corrupted or unallocated memory block"

#endif /* ERRORMSG_H */

