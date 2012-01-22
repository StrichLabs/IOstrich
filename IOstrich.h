/*
  IOstrich.h - Arduino library for the StrichLabs IOstrich Shield - description
  Copyright (c) 2011 Strich Labs.  Licensed under Creative Commons BY-NC-SA.
*/

// ensure this library description is only included once
#ifndef IOstrich_h
#define IOstrich_h

#if ARDUINO < 100
  #error This library requires Arduino 1.0 or later.  Please upgrade your development environment.
#else // ARDUINO < 100


// include core Arduino API
#include "Arduino.h"

#define NORMAL            0x00
#define INVERTED          0x01

#define BANK_A            0x00
#define BANK_B            0x01
#define BANK_C            0x02
#define BANK_D            0x03
#define BANK_E            0x04
  
#define REG_IN_BASE       0x00
#define REG_OUT_BASE      0x08
#define REG_INVERT_BASE   0x10
#define REG_IODIR_BASE    0x18
#define REG_MASKINT_BASE  0x20

// library interface description
class IOstrich {
  // user-accessible "public" interface
  public:
    IOstrich(byte);
    IOstrich(void);
    void pinMode(byte, byte, byte);
    void pinInvert(byte, byte, byte);
    void pinInterrupt(byte, byte, byte);
    void bankInterrupt(byte, byte);
    void digitalWrite(byte, byte, byte);
    byte digitalRead(byte, byte);
  
  // library-accessible "private" interface
  private:
    byte addr;
    byte iosWriteRegister(byte, byte);
    byte iosReadRegister(byte);
    void iosSendSubAddr(byte);
};

#endif // ARDUINO < 100
#endif // IOstrich_h
