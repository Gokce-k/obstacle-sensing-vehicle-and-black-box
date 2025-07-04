# obstacle-sensing-vehicle-and-black-box
This project is a Black Box Vehicle System using the PIC16F877A microcontroller. It detects obstacles with an ultrasonic sensor and uses a PIR sensor to confirm a collision. When a crash is detected, the system stops the motors, reads the current time from a DS3231 RTC module, calculates the vehicle's speed, and displays this information on an LCD screen.

The system uses various PIC features such as:

Timers for time measurement,

I2C for RTC communication,

PWM for motor control,

And digital inputs for obstacle and motion detection.

This project simulates a basic automotive black box, providing real-time monitoring and logging during a crash without using Arduino or external processors.
