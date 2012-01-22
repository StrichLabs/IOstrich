/*
  IOstrich.cpp - Arduino library for the StrichLabs IOstrich Shield - implementation
  Copyright (c) 2011 Strich Labs  Licensed under Creative Commons BY-NC-SA.
*/

// include this library's description file
#include "IOstrich.h"

// include description files for Wire, as the shield uses I2C
#include "Wire.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

// Description: Creates a variable of type IOstrich, to communicate with the shield.
// Syntax: IOstrich(address)
// Parameter: address - Address on the I2C bus of the shield to communicate with
// Returns: Instance of IOstrich associated with a specific shield.
IOstrich::IOstrich(byte shieldAddress) {
  addr = shieldAddress;
  Wire.begin();
}

IOstrich::IOstrich(void) {
  // this 'fake constructor' allows creating an 'empty' IOstrich type variable
  // later on, the 'real constructor' can be called to actually begin using it.
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Arduino sketches, this library, and other libraries

// Description: Configure the specified GPIO pin to be an input or an output.
// Syntax: IOstrichInstance.pinMode(bank, pin, direction);
// Parameter: bank - Bank containing GPIO pin to change
// Parameter: pin - GPIO pin number to change
// Parameter: direction - either INPUT or OUTPUT
// Returns: nothing
void IOstrich::pinMode(byte bank, byte pin, byte direction) {
  byte regDirs;
  regDirs = iosReadRegister(REG_IODIR_BASE + bank);
  if(direction == INPUT) {
    regDirs = regDirs | (1 << pin);
  } else {
    iosWriteRegister(REG_IODIR_BASE + bank, 0);
    regDirs = regDirs & ~(1 << pin);
  }
  iosWriteRegister(REG_IODIR_BASE + bank, regDirs);
}

// Description: Configure the specified GPIO pin to be inverted or not.  Inverted mode means that setting the pin to HIGH outputs 0v, and vice-versa.
// Syntax: IOstrichInstance.pinInvert(bank, pin, inverted);
// Parameter: bank - Bank containing GPIO pin to change
// Parameter: pin - GPIO pin number to change
// Parameter: inverted - NORMAL or INVERTED
// Returns: nothing
void IOstrich::pinInvert(byte bank, byte pin, byte inverted) {
  byte regInvert;
  regInvert = iosReadRegister(REG_INVERT_BASE + bank);
  if(inverted == INVERTED) {
    regInvert = regInvert | (1 << pin);
  } else {
//    iosWriteRegister(REG_INVERT_BASE + bank, 0);
    regInvert = regInvert & ~(1 << pin);
  }
  iosWriteRegister(REG_INVERT_BASE + bank, regInvert);
}

// Description: Configure the specified GPIO pin to generate interrupts on change or not.
// Syntax: IOstrichInstance.pinInterrupt(bank, pin, interrupt);
// Parameter: bank - Bank containing GPIO pin to change
// Parameter: pin - GPIO pin number to change
// Parameter: interrupt - 0 (do not generate interrupts on change) or 1 (generate interrupts on change)
// Returns: nothing
void IOstrich::pinInterrupt(byte bank, byte pin, byte interrupt) {
  byte regInterrupt;
  regInterrupt = iosReadRegister(REG_MASKINT_BASE + bank);
  if(interrupt == 0) {
    regInterrupt = regInterrupt | (1 << pin);
  } else {
    regInterrupt = regInterrupt & ~(1 << pin);
  }
  iosWriteRegister(REG_MASKINT_BASE + bank, regInterrupt);
}

// Description: Configure the specified bank to generate interrupts on change or not.
// Syntax: IOstrichInstance.bankInterrupt(bank, interrupt);
// Parameter: bank - Bank containing GPIO pins to change
// Parameter: interrupt - Byte containing 0 (do not generate interrupts on change) or 1 (generate interrupts on change) for each bit, each representing one pin.
// Returns: nothing
void IOstrich::bankInterrupt(byte bank, byte interrupt) {
  iosWriteRegister(REG_MASKINT_BASE + bank, ~interrupt);
}

// Description: Set a specified GPIO pin to a HIGH or LOW state.  Unlike the Arduino digital pins, these do not have pullup resistors, so digitalWrite when the pin is set to an input has no effect.
// Syntax: IOstrichInstance.digitalWrite(bank, pin, value)
// Parameter: bank - Bank containing GPIO pin to change
// Parameter: pin - GPIO pin number to change
// Parameter: value - HIGH or LOW
// Returns: nothing
void IOstrich::digitalWrite(byte bank, byte pin, byte val) {
  byte pins;

  pins = iosReadRegister(REG_OUT_BASE + bank);
  if(val == 1) {
    pins = pins | (1 << pin);
  } else {
    pins = pins & ~(1 << pin);
  }
  iosWriteRegister(REG_OUT_BASE + bank, pins);
}

// Description: Read the current value of a specific GPIO pin.
// Syntax: IOstrichInstance.digitalRead(bank, pin)
// Parameter: bank - Bank containing GPIO pin to read
// Parameter: pin - GPIO pin number to read the value of
// Returns: HIGH or LOW
byte IOstrich::digitalRead(byte bank, byte pin) {
  byte pins;
  
  pins = iosReadRegister(REG_IN_BASE + bank);
  
  if((pins & (1 << pin)) > 0) {
    return HIGH;
  } else {
    return LOW;
  }
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
byte IOstrich::iosWriteRegister(byte reg, byte val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission();
}

byte IOstrich::iosReadRegister(byte reg) {
  byte data;
  
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  data = Wire.read();
  return data;
}
 