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
const bool verbose        = true;

// Voltage mappings
const float minOut = 0;
const float cenOut = 511;
const float maxOut = 1023;


// Pins
const byte button0        = 8;
const byte button1        = 9;
const int xPin            = A2;
const int yPin            = A3;

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
const bool hasXAxis       = true;
const bool hasYAxis       = true;
const bool hasZAxis       = false;
const bool hasRxAxis      = false;
const bool hasRyAxis      = false;
const bool hasRzAxis      = false;
const bool hasRudder      = false;
const bool hasThrottle    = false;
const bool hasAccelerator = false;
const bool hasBrake       = false;
const bool hasSteering    = false;

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
    buttonState0 = digitalRead(button0);
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
    buttonState1 = digitalRead(button1);
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

void setXAxis() {
    const int vIn = analogRead(xPin);

    // Map the min/max analog values to the min/max axis values.
    const float minIn = 528;
    const float cenIn = 800;
    const float maxIn = 1020;

    int vOut;

    if (vIn <= cenIn) {
        //vOut = ((vIn - minIn)/(cenIn - minIn)) * cenOut;
        vOut = map(vIn, minIn, cenIn, minOut, cenOut);
    }
    else {  // v > center
        //vOut = ((vIn - cenIn)/(maxIn - cenIn)) * (cenOut + 1) + cenOut;
        vOut = map(vIn, cenIn, maxIn, cenOut, maxOut);
    }

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

void setOtherAxis() {
    const int vIn = analogRead(yPin);

    // Map the min/max analog values to the min/max axis values.
    const float minIn = 254;
    const float cenIn = 660;
    const float maxIn = 948;

    int vOut;

    if (vIn <= cenIn) {
        //vOut= ((vIn - minIn)/(cenIn - minIn)) * cenOut;
        vOut = map(vIn, minIn, cenIn, minOut, cenOut);
    }
    else {  // v > center
        //vOut = ((vIn - cenIn)/(maxIn - cenIn)) * (cenOut + 1) + cenOut;
        vOut = map(vIn, cenIn, maxIn, cenOut, maxOut);
    }

    int adjVOut = adjustVout(vOut);

    if (hasYAxis)       { Joystick.setYAxis       (adjVOut); }
    if (hasZAxis)       { Joystick.setZAxis       (adjVOut); }
    if (hasRxAxis)      { Joystick.setRxAxis      (adjVOut); }
    if (hasRyAxis)      { Joystick.setRyAxis      (adjVOut); }
    if (hasRzAxis)      { Joystick.setRzAxis      (adjVOut); }
    if (hasRudder)      { Joystick.setRudder      (adjVOut); }
    if (hasThrottle)    { Joystick.setThrottle    (adjVOut); }
    if (hasAccelerator) { Joystick.setAccelerator (adjVOut); }
    if (hasBrake)       { Joystick.setBrake       (adjVOut); }
    if (hasSteering)    { Joystick.setSteering    (adjVOut); }

    if (verbose) {
        Serial.print("[Speed] vIn: ");
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
    Joystick.setXAxisRange(minAxis, maxAxis);
    Joystick.setYAxisRange(minAxis, maxAxis);
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

