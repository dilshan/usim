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