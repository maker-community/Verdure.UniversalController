#include <Wire.h>
#include "MCP23017.h"


// Connect pin SCL of the expander to SCL
// Connect pin SDA of the expander to SDA
// don't solder A0,A1,A2 (default)
// Connect PA0 to a button，PA1 to a led


MCP23017 mcp;

void setup() {
    
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
    
    //Configue Port PA0 Output
    mcp.pinMode(0, INPUT);
    
    //turn on a 100K pullup internally
    mcp.pullUp(0, HIGH);  

    //use the PA1 LED as debugging
    mcp.pinMode(1, OUTPUT);
}



void loop() {
    // The LED will 'echo' the button
    mcp.digitalWrite(1, mcp.digitalRead(0));
}
