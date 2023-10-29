#include <Wire.h>
#include "MCP23017.h"

// Connect pin SCL of the expander to SCL
// Connect pin SDA of the expander to SDA
// don't solder A0,A1,A2 (default)
// Connect MCP23017 pin INTA or INTB to Arduino Pin 3
// Connect PA7 and PB7 to a button(when the button pressed,it connect to GND)

MCP23017 mcp;

// Interrupts from the MCP23017 will be handled by pin3
byte arduinoIntPin=3;

//this is interrupt vector
byte arduinoInterrupt=1;

volatile boolean awakenByInterrupt = false;

// Two pins at the MCP (Ports A/B where some buttons have been setup.)
// Buttons connect the pin to grond, and pins are pulled up.
byte mcpPinA=7;
byte mcpPinB=15;

void setup(){
  
  Serial.begin(9600);
  Serial.println("Interrupt Test Start");

  pinMode(arduinoIntPin,INPUT);

    /*the param can be 0 to 7,the default param is 7.means the dafault device address 0x27.
    Addr(BIN)  Addr(hex)   param
    010 0111    0x27        7
    010 0110    0x26        6
    010 0101    0x25        5
    010 0100    0x24        4
    010 0011    0x23        3
    010 0010    0x22        2
    010 0001    0x21        1
    010 0000    0x20        0
    */
  mcp.begin(7);
  
  // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
  // The INTA/B will not be Floating 
  // INTs will be signaled with a LOW
  mcp.setupInterrupts(true,false,LOW);

  // configuration for a button on port A
  // interrupt will triger when the pin is taken to ground by a pushbutton
  mcp.pinMode(mcpPinA, INPUT);
  mcp.pullUp(mcpPinA, HIGH);  // turn on a 100K pullup internally
  mcp.setupInterruptPin(mcpPinA,FALLING); 

  // similar, but on port B.
  mcp.pinMode(mcpPinB, INPUT);
  mcp.pullUp(mcpPinB, HIGH);  // turn on a 100K pullup internall
  mcp.setupInterruptPin(mcpPinB,FALLING);
  
}

// The int handler will just signal that the int has happen
// we will do the work from the main loop.
void intCallBack(){
  awakenByInterrupt=true;
}

void handleInterrupt(){
  
  // Get more information from the MCP from the INT
  uint8_t pin=mcp.getLastInterruptPin();
  uint8_t val=mcp.getLastInterruptPinValue();
  
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  Serial.println("the key has been pressed!!!");
  Serial.print("the last interrupt Pin is:");
  Serial.println(pin);
  Serial.print("the last interrupt value is:");
  Serial.println(val);
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");

  // we have to wait for the interrupt condition to finish
  // otherwise we might go to sleep with an ongoing condition and never wake up again.
  // as, an action is required to clear the INT flag, and allow it to trigger again.
  // see datasheet for datails.
  while( ! (mcp.digitalRead(mcpPinB) && mcp.digitalRead(mcpPinA) ));
  // and clean queued INT signal
  cleanInterrupts();
}

// handy for interrupts triggered by buttons
// normally signal a few due to bouncing issues
void cleanInterrupts(){
  EIFR=0x01;
  awakenByInterrupt=false;
}  

 
void loop(){
  // enable interrupts before going to sleep/wait
  // setup a callback for the arduino INT handler.
  attachInterrupt(arduinoInterrupt,intCallBack,FALLING);
  
  while(!awakenByInterrupt);
  
  // disable interrupts while handling them.
  detachInterrupt(arduinoInterrupt);
  
  if(awakenByInterrupt)
  {
    handleInterrupt();
  }
}


