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
const int minAnalog       = 0;    // Calibrate this value
const int maxAnalog       = 1023; // Calibrate this value
const int minAxis         = -127;
const int maxAxis         =  127;
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

// Joystick ctor params:
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

// Ctor
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

// Map the min/max analog values to the min/max axis values.
int mapAnalogValToAxisVal(int analogVal) {
    const int axisVal = map(analogVal, minAnalog, maxAnalog, minAxis, maxAxis);
    return axisVal;
}

void setButton0() {
    buttonState0 = !digitalRead(button0);
    Joystick.setButton(0, buttonState0);
    if (verbose) {
        Serial.print("setButton0 -> ");
        Serial.println(buttonState0);
    }
    //digitalWrite(13, buttonState0);
}

void setButton1() {
    buttonState1 = !digitalRead(button1);
    Joystick.setButton(1, buttonState1);
    if (verbose) {
        Serial.print("setButton1 -> ");
        Serial.println(buttonState1);
    }
    //digitalWrite(13, buttonState1);
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

void setXAxis() {
    int analogVal = analogRead(xPin);
    int axisVal = mapAnalogValToAxisVal(analogVal);
    Joystick.setXAxis(axisVal);

    if (verbose) {
        Serial.print("X: analog(");
        Serial.print(analogVal);
        Serial.print(") --> ");
        Serial.print("setXAxis(");
        Serial.print(axisVal);
        Serial.println(")");
    }
}

void setYAxis() {
    int analogVal = analogRead(yPin);
    int axisVal = mapAnalogValToAxisVal(analogVal);
    Joystick.setYAxis(axisVal);

    if (verbose) {
        Serial.print("Y: analog(");
        Serial.print(analogVal);
        Serial.print(") --> ");
        Serial.print("setYAxis(");
        Serial.print(axisVal);
        Serial.println(")");
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
    
    Joystick.setXAxisRange(minAxis, maxAxis);
    Joystick.setYAxisRange(minAxis, maxAxis);
    Joystick.begin();
}

void loop() {
    if (verbose) {
        delay(1000);
        Serial.println();
    }

    setButton0();
    setButton1();
    setXAxis();
    setYAxis();
}

