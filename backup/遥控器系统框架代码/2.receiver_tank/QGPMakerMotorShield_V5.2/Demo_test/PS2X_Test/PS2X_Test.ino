#include "PS2X_lib.h"  //for v1.6
#include "QGPMaker_Encoder.h"
#include "QGPMaker_MotorShield.h"

/******************************************************************
   set pins connected to PS2 controller:
     - 1e column: original
     - 2e colmun: Stef?
   replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        12  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        13  //17

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   true
//#define rumble      true
#define rumble      true

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

int lx = 0;
int ly =  0;

// Create the motor shield object with the default I2C address
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();

QGPMaker_DCMotor  *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor  *DCMotor_4 = AFMS.getMotor(4);



void setup() {

  Serial.begin(9600);
  AFMS.begin(50);
  Serial.println("===========beging==========");
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  int error = 0;
  do {
    error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
    if (error == 0) {
      Serial.println("\nConfigured successful! ");
      break;
    } else {
      delay(100);
    }
  } while (1);


  AFMS.begin(50);  // create with the default frequency 1.6KHz

}

void loop() {
  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  if (ps2x.Button(PSB_START))        //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");

  if (ps2x.Button(PSB_PAD_UP)) {     //will be TRUE as long as button is pressed
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    DCMotor_3->setSpeed(250);
    DCMotor_4->setSpeed(250);
    DCMotor_3->run(FORWARD);
    DCMotor_4->run(FORWARD);
  } else  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  } else  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  } else  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    DCMotor_3->setSpeed(250);
    DCMotor_4->setSpeed(250);
    DCMotor_3->run(BACKWARD);
    DCMotor_4->run(BACKWARD);
  } else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);
    DCMotor_4->setSpeed(0);
    DCMotor_4->run(RELEASE);
  }

  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("X-X-X-X");
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }

  vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
  if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
    if (ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if (ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
    if (ps2x.Button(PSB_TRIANGLE))
      Serial.println("Triangle pressed");
  }

  if (ps2x.ButtonPressed(PSB_CIRCLE))              //will be TRUE if button was JUST pressed
    Serial.println("Circle just pressed");
  if (ps2x.NewButtonState(PSB_CROSS))              //will be TRUE if button was JUST pressed OR released
    Serial.println("X just changed");
  if (ps2x.ButtonReleased(PSB_SQUARE))             //will be TRUE if button was JUST released
    Serial.println("Square just released");

  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
  }
  delay(50);
}
