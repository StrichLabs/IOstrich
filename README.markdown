## IOstrich()

### Description
Creates a variable of type IOstrich, to communicate with the shield.

### Syntax
IOstrich(*address*)

### Parameters
*address* - Address on the I2C bus of the shield to communicate with

### Returns
Instance of IOstrich associated with a specific shield.

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at address 0x20
    IOstrich iosInstance = IOstrich(0x20);

## pinMode()

### Description
Configure the specified GPIO pin to be an input or an output.

### Syntax
*IOstrichInstance*.pinMode(*bank*, *pin*, *direction*);

### Parameters
*bank* - Bank containing GPIO pin to change
*pin* - GPIO pin number to change
*direction* - either INPUT or OUTPUT

### Returns
nothing

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      // Configure bank A GPIO pin 0 as an output, 1 as an input
      iosInstance.pinMode(BANK_A, 0, OUTPUT);
      iosInstance.pinMode(BANK_A, 1, INPUT);
    }
## pinInvert()

### Description
Configure the specified GPIO pin to be inverted or not.  Inverted mode means that setting the pin to HIGH outputs 0v, and vice-versa.

### Syntax
*IOstrichInstance*.pinInvert(*bank*, *pin*, *inverted*);

### Parameters
*bank* - Bank containing GPIO pin to change
*pin* - GPIO pin number to change
*inverted* - NORMAL or INVERTED

### Returns
nothing

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      // Configure bank A GPIO pin 0 to be inverted (digitalWrite to 1 makes it output low)
      iosInstance.pinMode(BANK_A, 0, INVERTED);
      // Configure bank A GPIO pin 0 to be non-inverted (which is the default)
      iosInstance.pinMode(BANK_A, 1, NORMAL);
    }
## pinInterrupt()

### Description
Configure the specified GPIO pin to generate interrupts on change or not.

### Syntax
*IOstrichInstance*.pinInterrupt(*bank*, *pin*, *interrupt*);

### Parameters
*bank* - Bank containing GPIO pin to change
*pin* - GPIO pin number to change
*interrupt* - 0 (do not generate interrupts on change) or 1 (generate interrupts on change)

### Returns
nothing

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      // Configure bank A GPIO pin 0 to generate interrupts on change
      iosInstance.pinInterrupt(BANK_A, 0, 1);
    }

## bankInterrupt()

### Description
Configure the specified bank to generate interrupts on change or not.

### Syntax
*IOstrichInstance*.bankInterrupt(*bank*, *interrupt*);

### Parameters
*bank* - Bank containing GPIO pins to change
*interrupt* - Byte containing 0 (do not generate interrupts on change) or 1 (generate interrupts on change) for each bit, each representing one pin.

### Returns
nothing

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      // Configure bank A GPIO pin 0 and 2 to generate interrupts on change
      iosInstance.bankInterrupt(BANK_A, 0x05);  // 00000101
    }

## digitalWrite()

### Description
Set a specified GPIO pin to a HIGH or LOW state.  Unlike the Arduino digital pins, these do not have pullup resistors, so digitalWrite when the pin is set to an input has no effect.

### Syntax
*IOstrichInstance*.digitalWrite(*bank*, *pin*, *value*)

### Parameters
*bank* - Bank containing GPIO pin to change
*pin* - GPIO pin number to change
*value* - HIGH or LOW

### Returns
nothing

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      // Configure bank A GPIO pin 0 as an output
      iosInstance.pinMode(BANK_A, 0, OUTPUT);
      
      // Set bank A GPIO pin 0 to a HIGH level
      iosInstance.digitalWrite(BANK_A, 0, HIGH);
    }
## digitalRead()

### Description
Read the current value of a specific GPIO pin.

### Syntax
*IOstrichInstance*.digitalRead(*bank*, *pin*)

### Parameters
*bank* - Bank containing GPIO pin to read
*pin* - GPIO pin number to read the value of

### Returns
HIGH or LOW

### Example
    #include <IOstrich.h>
    
    // Create an IOstrich instance to communicate with the shield at 0x20
    IOstrich iosInstance = IOstrich(0x20);
    
    void setup() {
      byte pinLevel;
      
      // Configure bank A GPIO pin 0 as an input
      iosInstance.pinMode(BANK_A, 0, INPUT);
      
      // Read the value on bank A GPIO pin 0 and put it into pinLevel
      pinLevel = iosInstance.digitalRead(BANK_A, 0);
    }
