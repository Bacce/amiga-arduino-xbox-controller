//TODO:
// fix mouse analog controls

#include <XBOXONE.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXONE Xbox(&Usb);

//CONFIG
bool logs = 0;

//RUNTIME VARIABLE
bool autofire = 0;
bool directionUsed = 0;
int sensi = 7500; //Analog stick sensibility
int mode = 1;

//Mouse support
int xspeed = 1;
int yspeed = 1;

static int pin_xa = 2; //Mouse V-Pulse DB9 - pin 2
static int pin_xb = 6; //Mouse H-Pulse DB9 - pin 6
static int pin_ya = 5; //Moues VQ-Pulse DB9 - pin 5
static int pin_yb = 7; //Mouse HQ-Pulse DB9 - pin 7


void setPinToJoy(){ //mode 1
  pinMode(2, INPUT); //Joy Up DB9 - pin 1
  pinMode(5, INPUT); //Joy Down DB9 - pin 2
  pinMode(6, INPUT); //Joy Left DB9 - pin 3
  pinMode(7, INPUT); //Joy Right DB9 - pin 4
  pinMode(8, INPUT); //Joy Button 1 BD9 - pin 6 (row 2 first)
  pinMode(A0, INPUT); //Joy Button 2 DB9 - pin 9 (row 2 last)
}

void setPinToMouse(){ //mode 0
  pinMode(pin_xa, INPUT);
  pinMode(pin_xb, INPUT);
  pinMode(pin_ya, INPUT);
  pinMode(pin_yb, INPUT);
}

void setup() {
  if(logs) {
    Serial.begin(9600);
    if (Usb.Init() == -1) {
      Serial.print(F("\r\nOSC did not start"));
      while (1); //halt
    }
    Serial.print(F("\r\nXBOX USB Library Started"));
  }
  else {
    Usb.Init();
  }

  //Joystick config
  setPinToJoy();
}

void rumble(int force=100, int time=30) {
    uint8_t rumble = force;
    uint8_t rest = 0;
    Xbox.setRumbleOn(rest, rumble, rest, rest);
    delay(time);
    Xbox.setRumbleOff();
}

void buttonPress(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  if(logs) { Serial.print("button pressed"); Serial.println(pin); }
}
void buttonRelease(uint8_t pin){ pinMode(pin, INPUT); }

void mouseDown(int speed = yspeed) {
  pinMode(pin_xa, INPUT);
  digitalWrite(pin_xa, LOW);
  delay(speed);
  pinMode(pin_xb, INPUT);
  digitalWrite(pin_xb, LOW);
  delay(speed);
  pinMode(pin_xa, OUTPUT);
  delay(speed);
  pinMode(pin_xb, OUTPUT);
  delay(speed);
}

void mouseUp(int speed = xspeed) {
  pinMode(pin_xb, INPUT);
  digitalWrite(pin_xb, LOW);
  delay(speed);
  pinMode(pin_xa, INPUT);
  digitalWrite(pin_xa, LOW);
  delay(speed);
  pinMode(pin_xb, OUTPUT);
  delay(speed);
  pinMode(pin_xa, OUTPUT);
  delay(speed);
}

void mouseLeft(int speed = yspeed) {
  pinMode(pin_yb, INPUT);
  digitalWrite(pin_yb, LOW);
  delay(speed);
  pinMode(pin_ya, INPUT);
  digitalWrite(pin_ya, LOW);
  delay(speed);
  pinMode(pin_yb, OUTPUT);
  delay(speed);
  pinMode(pin_ya, OUTPUT);
  delay(speed);
}

void mouseRight(int speed = xspeed) {
  pinMode(pin_ya, INPUT);
  digitalWrite(pin_ya, LOW);
  delay(speed);
  pinMode(pin_yb, INPUT);
  digitalWrite(pin_yb, LOW);
  delay(speed);
  pinMode(pin_ya, OUTPUT);
  delay(speed);
  pinMode(pin_yb, OUTPUT);
  delay(speed);
}


void loop() {
  Usb.Task();
  
  if (Xbox.XboxOneConnected) {
    if(mode == 0){ //Mouse mode
      directionUsed = 0;
      int xPos = Xbox.getAnalogHat(LeftHatX);
      int yPos = Xbox.getAnalogHat(LeftHatY);
  
      if(xPos > sensi) {
        int speed = map(xPos, 7500, 32700, 20, 1);
        mouseRight(speed);
        directionUsed = 1;
      }
      else if(xPos < -sensi) {
        int speed = map(xPos, -7500, -32700, 20, 1);
        mouseLeft(speed);
        directionUsed = 1;
      }

      if(yPos > sensi) {
        int speed = map(yPos, 7500, 32700, 20, 1);
        mouseUp(speed);
        directionUsed = 1;
      }
      else if(yPos < -sensi) {
        int speed = map(yPos, -7500, -32700, 20, 1);
        mouseDown(speed);
        directionUsed = 1;
      }

      if(directionUsed == 0) {
        if (Xbox.getButtonPress(UP)) { mouseUp();}
        if (Xbox.getButtonPress(DOWN)) { mouseDown();}
        if (Xbox.getButtonPress(LEFT)) { mouseLeft(); }
        if (Xbox.getButtonPress(RIGHT)) { mouseRight(); }
      }

      //Left click
      if (Xbox.getButtonPress(A)) { buttonPress(8); }
      else { buttonRelease(8); }

      //Right click
      if (Xbox.getButtonPress(B)) { buttonPress(A0); }
      else { buttonRelease(A0); }
    }
    else if(mode == 1){ //Joystick mode    
      directionUsed = 0;
      int xPos = Xbox.getAnalogHat(LeftHatX);
      int yPos = Xbox.getAnalogHat(LeftHatY);
  
      if(xPos > sensi) {
        buttonPress(7);
        buttonRelease(6);
        directionUsed = 1;
        if(logs) { Serial.println("Right");}
      }
      else if(xPos < -sensi) {
        buttonPress(6);
        buttonRelease(7);
        directionUsed = 1;
        if(logs) { Serial.println("Left");}
      }
      else {
        buttonRelease(7);
        buttonRelease(6);
        if(logs) { Serial.println("center X");}
      }
  
      if(yPos > sensi) {
        buttonPress(2);
        buttonRelease(5);
        directionUsed = 1;
        if(logs) { Serial.println("Up");}
      }
      else if(yPos < -sensi) {
        buttonPress(5);
        buttonRelease(2);
        directionUsed = 1;
        if(logs){Serial.println("Down");}
      }
      else {
        buttonRelease(5);
        buttonRelease(2);
        if(logs) { Serial.println("center Y");}
      }
  
      if(directionUsed == 0) {
        if (Xbox.getButtonPress(UP)) { buttonPress(2);}
        else { buttonRelease(2);}
        
        if (Xbox.getButtonPress(DOWN)) { buttonPress(5);}
        else { buttonRelease(5);}
        
        if (Xbox.getButtonPress(LEFT)) { buttonPress(6); }
        else { buttonRelease(6); }
          
        if (Xbox.getButtonPress(RIGHT)) { buttonPress(7); }
        else { buttonRelease(7);}
      }
  
      if (Xbox.getButtonPress(A)) { buttonPress(8); }
      else { buttonRelease(8); }
  
      if (Xbox.getButtonPress(B)) { buttonPress(A0); }
      else { buttonRelease(A0); }
  
      if (Xbox.getButtonClick(X)) {
        if(autofire == 1) {
          autofire = 0;
          rumble();
        }
        else {
          autofire = 1;
          rumble(200, 80);
        }
      }
  
      if(autofire == 1) {
        buttonPress(8);
        delay(20);
        buttonRelease(8);
      }
  
      if (Xbox.getButtonClick(Y)) {}
    }

    if (Xbox.getButtonClick(XBOX)) {
      if(mode == 1) {
        mode = 0;
        setPinToJoy();
        rumble();
      }
      else {
        mode = 1;
        setPinToMouse();
        rumble();
      }
    }
  }

  delay(5);
}