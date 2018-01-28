//------------------------------------------------------------
// A joystick HID program for a Promicro controller.
//
// A0 - enable joy
// A3 - x-axis
//
// David Ohlemacher
//------------------------------------------------------------

#include "Joystick.h"

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

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
const bool testAutoSendMode = false;

const int xPin = 3;

const byte button0 = 0;
byte button0State = 0;
const byte buttonDown = 0;
const byte buttonUp = 1;

int x = -127;
int y = -127;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcButtonDelta = 500;
const unsigned long gcAnalogDelta = 25;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;

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

void setXAxis() {
    int ival = analogRead(xPin);
    // map(value, fromLow, fromHigh, toLow, toHigh)
    int val = map(ival, 0, 1023, -127, 127);
    Serial.print("analog(");
    Serial.print(ival);
    Serial.print(") --> ");
    Serial.print("setXAxis(");
    Serial.print(val);
    Serial.println(")");
    Joystick.setXAxis(val);
    //Joystick.setYAxis(val);
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
    pinMode(A0, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP); 
    pinMode(13, OUTPUT);
    
    Joystick.setXAxisRange(-127, 127);
    Joystick.begin();
}

void loop() {
    // TODO - Slow it down a bit, but remove
    delay(1000);

    // System Disabled
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
}

