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

// Slow down loop(). Be verbose. For debugging only.
const bool verbose        = false;

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
}

void loop() {
    if (verbose) {
        delay(1500);
        Serial.println();
    }

    setButton0();
    setButton1();
    setXAxis();
    setOtherAxis();
}

