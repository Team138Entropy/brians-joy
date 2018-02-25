# Joystick code

## References
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary/blob/master/README.md)
- [WPI Joystick](http://first.wpi.edu/FRC/roborio/release/docs/java/edu/wpi/first/wpilibj/Joystick.html)
- [Joystick Instructable](http://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
- [Installing Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries)

## Naming the joystick
Unfortunately I couldn’t figure out how it can be influenced from within the sketch.  You can change the name globally by modifying the “boards.txt” file.

boards.txt can be found in the Arduino installation folder.
`.../Arduino/hardware/sparkfun/avr/boards.txt`

This will modify the name for all devices programmed. Find the "Pro Micro" section.Change it:
- `promicro.name=Brians Joystick`
- `promicro.build.usb_product="Brians Joystick"` 

## I2C Communications to an accelerometer 
- Accelerometer:
    - Divesified Systems - ADXL345
    - Seems to a First specific device. Little documentation found.
- Pins:
    - 
- Library:
    - [Wire - I2C library](https://www.arduino.cc/en/Reference/Wire)
