# Joystick code

## References
- [Pro Micro Hookup Guide](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro)
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary/blob/master/README.md)
- [WPI Joystick](http://first.wpi.edu/FRC/roborio/release/docs/java/edu/wpi/first/wpilibj/Joystick.html)
- [Joystick Instructable](http://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
- [Installing Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries)

## Naming the joystick
Unfortunately I couldn’t figure out how the joystick name can be influenced from within the sketch.  You can change the name globally by modifying the “boards.txt” file.

boards.txt can be found in the Arduino installation folder.
`.../Arduino/hardware/sparkfun/avr/boards.txt`

This will modify the name for all devices programmed. Find the "Pro Micro" section.Change it:
- `promicro.name=Brians Joystick`
- `promicro.build.usb_product="Brians Joystick"`

## I2C Communications to an accelerometer
- Ref:
    - [ADXL345 Hookup Guide](https://learn.sparkfun.com/tutorials/adxl345-hookup-guide)

- Accelerometer:
    - Divesified Systems - ADXL345
    - Seems to a First specific device. Little documentation found.
- Pins for Pro Micro:
    - 2: SDA
    - 3: SCL
- Library:
    - [Wire - I2C library](https://www.arduino.cc/en/Reference/Wire)
    - Sparkfun ADXL345 Arduino Library by Sparkfun Electronics v. 1.0.0
        - Installed via library manager.
		- Set the device ID in SparkFun_ADXL345.cpp:
            - `#define ADXL345_DEVICE (0x1D)  // Device Address for ADXL345`
            - Find the I2C device ID using i2c_scanner.ino. For me, it was 0x1D.
