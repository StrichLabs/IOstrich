#include <Wire.h>
#include <IOstrich.h>

// IOstrich GPIO Shield test program
// Requires a loopback adapter connected to each bank header, connecting 0->1, 2->3, 4->5, and 6->7

// keep track of how many interrupts trigger during the test
volatile byte numInterrupts;

void interruptHandler(void) {
  numInterrupts++;
}

void setup(void) {
  byte addr;
  numInterrupts = 0;
  
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\n\nIOstrich Board Test");
  
  // run through each possible board address, testing each one that is found
  for(addr=0x20; addr < 0x28; addr++) {
    Wire.beginTransmission(addr);
    Wire.write((byte)0x00);
    Serial.print("0x");
    Serial.print(addr, HEX);
    if(Wire.endTransmission() != 0) {
      Serial.println(": No shield found.");
      continue;
    }
    Serial.println(": Shield found, testing...");
    testShield(addr);
  }
}

void loop(void) {
  // we only test once, so just spin here forever
}

// run through all tests for one shield
void testShield(byte addr) {
  IOstrich ios = IOstrich(addr);
  byte numFails = 0, bank = 0;

  // turn off all interrupts and make sure nothing generates any  
  for(bank=0; bank<5; bank++) {
    ios.bankInterrupt(bank, 0x00);
  }
  // set interrupt pin 0 (digital 2) to an input with the pull-up enabled
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(0, &interruptHandler, FALLING);
  for(bank=0; bank<5; bank++) {
    numFails += runBankTest(ios, bank);
  }
  
  numFails += runInterruptTest(ios);
  
  if(numFails == 0) {
    Serial.println(" Test PASSED with no errors.");
  } else {
    Serial.print(" Test FAILED with ");
    Serial.print(numFails, DEC);
    Serial.println(" errors.");
  }
}

// run an I/O test on a single bank, to make sure all inputs/outputs function properly
byte runBankTest(IOstrich ios, int bank) {
  Serial.print(" I/O Bank ");
  Serial.write('A' + bank);
  Serial.print("...");
  
  byte pin, baseOutputPin, numFails = 0;
  // first time through the test outputs on even numbered pins and inputs on odd numbered pins
  // first time through the test outputs on odd numbered pins and inputs on even numbered pins
  for(baseOutputPin=0; baseOutputPin < 2; baseOutputPin++) {
    for(pin=baseOutputPin; pin<8; pin+=2) {
      byte outputPin = pin;
      byte inputPin = pin + 1;
      if(baseOutputPin == 1) inputPin = pin - 1;
      
      ios.pinMode(bank, inputPin, INPUT);
      ios.pinMode(bank, outputPin, OUTPUT);
            
      Serial.print(".");
      ios.digitalWrite(bank, outputPin, HIGH);
      if(ios.digitalRead(bank, inputPin) != HIGH) {
/*        Serial.print("Input pin ");
        Serial.print(inputPin, DEC);
        Serial.println(" stuck low?");*/
        numFails++;
      }
      Serial.print(".");
      ios.digitalWrite(bank, outputPin, LOW);
      if(ios.digitalRead(bank, inputPin) != LOW) {
/*        Serial.print("Input pin ");
        Serial.print(inputPin, DEC);
        Serial.println(" stuck high?");*/
        numFails++;
      }
    }
  }
  
  if(numFails > 0) {
    Serial.print(" FAIL: ");
    Serial.print(numFails, DEC);
    Serial.println(" tests failed.");
  } else {
    Serial.println(" PASS!");
  }
  
  return numFails;
}

// run the interrupt test on a shield, to make sure interrupt-on-change functions properly
byte runInterruptTest(IOstrich ios) {
  byte numFails = 0;
  
  Serial.print(" Interrupts................... ");
  if(numInterrupts > 0) {
    Serial.println("FAIL: Unexpected interrupts received!");
    numFails++;
  } else {
    ios.pinMode(BANK_E, 0, INPUT);
    ios.pinMode(BANK_E, 1, OUTPUT);
    ios.digitalWrite(BANK_E, 1, LOW);
    ios.pinInterrupt(BANK_E, 0, 1);
    delay(500);
    if(numInterrupts > 0) {
      Serial.println("FAIL: Unexpected interrupts received after unmask!");
      numFails++;
    } else {
      ios.digitalWrite(BANK_E, 1, HIGH);
      delay(10);
      if(numInterrupts != 1) {
        Serial.print("FAIL: Interrupt triggered ");
        Serial.print(numInterrupts, HEX);
        Serial.println(" times, should be 1.");
        numFails++;
      }
    }
  }
  if(numFails == 0) {
    Serial.println("PASS!");
  }
  return numFails;
}