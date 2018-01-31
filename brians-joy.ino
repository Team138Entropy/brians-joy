//------------------------------------------------------------
// A joystick HID program for a Promicro controller.
//
// A0 - enable joy
// A1 - x-axis
// A2 - x-axis
//
// David Ohlemacher, 2018, Power Up Season
//------------------------------------------------------------

#include "Joystick.h"

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
const bool testAutoSendMode = false;
// Slow down the loop() for debugging.
const bool turtleMode = false;
// Pins
const int xPin = 1;
const int yPin = 2;
// Buttons
const byte button0 = 0;
byte button0State = 0;
const byte buttonDown = 0;
const byte buttonUp = 1;
// Voltage mappings
const int minAnalog = 0;
const int maxAnalog = 1023;
const int minAxis = -127;
const int maxAxis =  127;

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

void testSingleButtonPush(unsigned int button)
{
    Serial.print("testSingleButtonPush");
    if (button > 0)
    {
        Joystick.releaseButton(button - 1);
    }
    if (button < 4)
    {
        Joystick.pressButton(button);
    }
}

void testMultiButtonPush(unsigned int currentStep) {
    Serial.print("testMultiButtonPush");
    for (int button = 0; button < 4; button++)
    {
        if ((currentStep == 0) || (currentStep == 2))
        {
            if ((button % 2) == 0)
            {
                Joystick.pressButton(button);
            }
            else if (currentStep != 2)
            {
                Joystick.releaseButton(button);
            }
        } // if ((currentStep == 0) || (currentStep == 2))
        if ((currentStep == 1) || (currentStep == 2))
        {
            if ((button % 2) != 0)
            {
                Joystick.pressButton(button);
            }
            else if (currentStep != 2)
            {
                Joystick.releaseButton(button);
            }
        } // if ((currentStep == 1) || (currentStep == 2))
        if (currentStep == 3)
        {
            Joystick.releaseButton(button);
        } // if (currentStep == 3)
    } // for (int button = 0; button < 32; button++)
}

void testAcceleratorBrake(int value) {
    Serial.print("testAcceleratorBrake");
    Joystick.setAccelerator(value);
    Joystick.setBrake(260 - value);
}

void testSteering(int value) {
    Serial.print("testSteering");
    if (value < 300) {
        Joystick.setSteering(value);
    }
    else {
        Joystick.setSteering(600 - value);
    }
}

int mapAnalogValToAxisVal(int analogVal) {
    const int axisVal = map(analogVal, minAnalog, maxAnalog, minAxis, maxAxis);
    return axisVal;
}

void setXAxis() {
    int analogVal = analogRead(xPin);
    int axisVal = mapAnalogValToAxisVal(analogVal);
    Serial.print("X: analog(");
    Serial.print(analogVal);
    Serial.print(") --> ");
    Serial.print("setXAxis(");
    Serial.print(axisVal);
    Serial.println(")");
    Joystick.setXAxis(axisVal);
    //Joystick.setYAxis(val);
}

void setYAxis() {
    int analogVal = analogRead(yPin);
    int axisVal = mapAnalogValToAxisVal(analogVal);
    Serial.print("Y: analog(");
    Serial.print(analogVal);
    Serial.print(") --> ");
    Serial.print("setYAxis(");
    Serial.print(axisVal);
    Serial.println(")");
    Joystick.setYAxis(axisVal);
}

void toggleButton0() {
    if (button0State == buttonUp) {
        button0State = buttonDown;
    }
    else {
        button0State = buttonUp;
    }
    Serial.print("toggleButton0(");
    Serial.print(button0State);
    Serial.println(")");
    Joystick.setButton(button0, button0State);
}

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT_PULLUP); // enable
    pinMode(A1, INPUT_PULLUP); // x
    pinMode(A2, INPUT_PULLUP); // y
    pinMode(13, OUTPUT);
    
    Joystick.setXAxisRange(minAxis, maxAxis);
    Joystick.setYAxisRange(minAxis, maxAxis);
    Joystick.begin();
}

void loop() {
    if (turtleMode) {
        delay(1000);
    }

    // Sense if enabled
    if (digitalRead(A0) != 0)
    {
        // Turn indicator light off.
        digitalWrite(13, 0);
        Serial.print("-");
        return;
    }
    // Turn indicator light on.
    digitalWrite(13, 1);
    Serial.println("+");

    setXAxis();
    setYAxis();
}

