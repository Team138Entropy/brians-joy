//------------------------------------------------------------
// A joystick HID program for a Promicro controller.
//
// A0 - button 0
// A1 - button 1
// A2 - x-axis
// A3 - x-axis
//
// David Ohlemacher, 2018, Power Up Season
//------------------------------------------------------------

#include "Joystick.h"
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library

// Slow down loop(). Be verbose. For debugging only.
const bool verbose        = true;

// Voltage mappings
const float minOut = 0;
const float cenOut = 511;
const float maxOut = 1023;

// Pins
const byte button0        = 8;
const byte button1        = 9;
const int xPin            = A3;
const int yPin            = A2;

// Buttons
int buttonState0          = 0;
int buttonState1          = 0;
const byte buttonDown     = 0;
const byte buttonUp       = 1;

// Joystick constructor params:
const byte hidReportId    = 3;  // Do not use 1 (mouse) or 2 (keyboard)
const byte joystickType   = JOYSTICK_TYPE_JOYSTICK;
const byte buttonCount    = 4;
const byte hatSwitchCount = 0;
const bool hasXAxis       = true;  // x
const bool hasYAxis       = false; // y
const bool hasZAxis       = false; // z
const bool hasRxAxis      = true;  // x rotate
const bool hasRyAxis      = false; // y rotate
const bool hasRzAxis      = false; // z rotate
const bool hasRudder      = false; //   z rotate
const bool hasThrottle    = false; //   slider
const bool hasAccelerator = false; //   y
const bool hasBrake       = false; //   z rotate
const bool hasSteering    = false; // ?

// Construct a joystick object
Joystick_ Joystick(
        hidReportId, 
        joystickType,
        buttonCount,
        hatSwitchCount,
        hasXAxis,
        hasYAxis,
        hasZAxis,
        hasRxAxis,
        hasRyAxis,
        hasRzAxis,
        hasRudder,
        hasThrottle,
        hasAccelerator,
        hasBrake,
        hasSteering);

ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

void setButton0() {
    buttonState0 = ! digitalRead(button0);
    Joystick.setButton(0, buttonState0);
    if (verbose) {
        if (buttonState0 == buttonUp) {
            Serial.println("Button 0: OFF");
        }
        else {
            Serial.println("Button 0: ON");
        }
    }
}

void setButton1() {
    buttonState1 = ! digitalRead(button1);
    Joystick.setButton(1, buttonState1);
    if (verbose) {
        if (buttonState1 == buttonUp) {
            Serial.println("Button 1: OFF");
        }
        else {
            Serial.println("Button 1: ON");
        }
    }
}

// Not used
void toggleButton0() {
    if (buttonState0 == buttonUp) {
        buttonState0 = buttonDown;
    }
    else {
        buttonState0 = buttonUp;
    }
    if (verbose) {
        Serial.print("toggleButton0(");
        Serial.print(buttonState0);
        Serial.println(")");
    }
    Joystick.setButton(button0, buttonState0);
}

// If vOut is near center, center it.
int adjustVout(int vOut) {
    int vAdj = vOut;
    const int eps = 15;
    if (vOut < eps) {
        vAdj = minOut;
    }
    if (vOut > (maxOut-eps)) {
        vAdj = maxOut;
    }
    if ((vOut > (cenOut-eps)) && (vOut < (cenOut+eps))) {
        vAdj = cenOut;
    }

    return vAdj;
}

// Set speed axis.
void setXAxis() {
    const int vIn = analogRead(xPin);

    // Map the min/max analog values to the min/max axis values.
    const float minIn = 254;
    const float cenIn = 660;
    const float maxIn = 948;

    int vOut;

    if (vIn <= cenIn) {
        //vOut = ((vIn - minIn)/(cenIn - minIn)) * cenOut;
        vOut = map(vIn, minIn, cenIn, minOut, cenOut);
    }
    else {  // v > center
        //vOut = ((vIn - cenIn)/(maxIn - cenIn)) * (cenOut + 1) + cenOut;
        vOut = map(vIn, cenIn, maxIn, cenOut, maxOut);
    }

    if (hasXAxis) {
        int adjVOut = adjustVout(vOut);
        Joystick.setXAxis(adjVOut);

        if (verbose) {
            Serial.print("[Turn ] vIn: ");
            Serial.print(vIn);
            Serial.print(" --> vOut: ");
            Serial.print(vOut);
            if (vOut != adjVOut) {
                Serial.print(" -> adjusted: ");
                Serial.print(adjVOut);
            }
            Serial.println();
        }
    }
}

void setOtherAxis() {
    const int vIn = analogRead(yPin);

    // Map the min/max analog values to the min/max axis values.
	const float minIn = 528; // speed
    const float cenIn = 800;
    const float maxIn = 1020;

    int vOut;

    // Compensate for the center being offset.
    // Split and map each half.
    if (vIn <= cenIn) {
        //vOut= ((vIn - minIn)/(cenIn - minIn)) * cenOut;
        vOut = map(vIn, minIn, cenIn, minOut, cenOut);
    }
    else {  // v > center
        //vOut = ((vIn - cenIn)/(maxIn - cenIn)) * (cenOut + 1) + cenOut;
        vOut = map(vIn, cenIn, maxIn, cenOut, maxOut);
    }

    int adjVOut = adjustVout(vOut);
    String ax = String("");

    if (hasYAxis)       { Joystick.setYAxis       (adjVOut); ax+="Yy"; }
    if (hasZAxis)       { Joystick.setZAxis       (adjVOut); ax+="Zz"; }
    if (hasRxAxis)      { Joystick.setRxAxis      (adjVOut); ax+="Rx"; }
    if (hasRyAxis)      { Joystick.setRyAxis      (adjVOut); ax+="Ry"; }
    if (hasRzAxis)      { Joystick.setRzAxis      (adjVOut); ax+="Rz"; }
    if (hasRudder)      { Joystick.setRudder      (adjVOut); ax+="Ru"; }
    if (hasThrottle)    { Joystick.setThrottle    (adjVOut); ax+="Th"; }
    if (hasAccelerator) { Joystick.setAccelerator (adjVOut); ax+="Ac"; }
    if (hasBrake)       { Joystick.setBrake       (adjVOut); ax+="Br"; }
    if (hasSteering)    { Joystick.setSteering    (adjVOut); ax+="St"; }

    if (verbose) {
        Serial.print("[Speed ");
        Serial.print(ax);
        Serial.print("] vIn: ");
        Serial.print(vIn);
        Serial.print(" --> vOut: ");
        Serial.print(adjVOut);
        if (vOut != adjVOut) {
            Serial.print(" -> adjusted: ");
            Serial.print(adjVOut);
        }
        Serial.println();
    }
}

void setTurboMode() {
    // Accelerometer Readings
    int x,y,z;
    adxl.readAccel(&x, &y, &z); // Read the accelerometer values and store them in variables declared above x,y,z

    int isTurboOn = 0;

	if (x >= 20) {
	    isTurboOn = 1;
	}
	Joystick.setButton(1, isTurboOn);

    if (verbose) {
		Serial.print("Turbo: ");
		Serial.print(isTurboOn);
		Serial.print(" [x=");
		Serial.print(x);
		Serial.print(" y=");
		Serial.print(y);
		Serial.print(" z=");
		Serial.print(z);
		Serial.println("]");
	}
}

void setup_adxl() {

  Serial.begin(9600);                 // Start the serial terminal
  Serial.println("SparkFun ADXL345 Accelerometer Hook Up Guide Example");
  Serial.println();

  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment

  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.

  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);

//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt

}

void setup() {
    if (verbose) {
        Serial.begin(9600);
    }
    pinMode(button0, INPUT_PULLUP);
    pinMode(button1, INPUT_PULLUP);
    pinMode(xPin,    INPUT_PULLUP); // x
    pinMode(yPin,    INPUT_PULLUP); // y

    // Indicator light
    pinMode(13, OUTPUT);
    //digitalWrite(13, 1);
    
    const int minAxis = 0;
    const int maxAxis = 1024;
    if (hasXAxis) {
        Joystick.setXAxisRange(minAxis, maxAxis);
    }
    if (hasYAxis) {
        Joystick.setYAxisRange(minAxis, maxAxis);
    }
    if (hasZAxis) {
        Joystick.setZAxisRange(minAxis, maxAxis);
    }
    if (hasRxAxis) {
        Joystick.setRxAxisRange(minAxis, maxAxis);
    }
    if (hasRyAxis) {
        Joystick.setRyAxisRange(minAxis, maxAxis);
    }
    if (hasRzAxis) {
        Joystick.setRzAxisRange(minAxis, maxAxis);
    }
    if (hasRudder) {
        Joystick.setRudderRange(minAxis, maxAxis);
    }
    if (hasThrottle) {
        Joystick.setThrottleRange(minAxis, maxAxis);
    }
    if (hasAccelerator) {
        Joystick.setAcceleratorRange(minAxis, maxAxis);
    }
    if (hasBrake) {
        Joystick.setBrakeRange(minAxis, maxAxis);
    }
    if (hasSteering) {
        Joystick.setSteeringRange(minAxis, maxAxis);
    }

    Joystick.begin();

    setup_adxl();
}

void loop() {
    if (verbose) {
        delay(1500);
        Serial.println();
    }

    setButton0();
    setTurboMode();
    setXAxis();
    setOtherAxis();
}

