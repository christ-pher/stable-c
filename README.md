# stable-c
### Object Stabilization in C/C++
*Current idea:* Create a DIY arduino drone and program the control/stabilization logic in C.

- ***drone.c*** the first POC that just outlined the basic logic of drone controls. This set the foundation for the control logic.
- ***stabilization.c*** the second POC to attempt to compensate for angular changes of a drone in mid air. The logic is a theoretical solution to keeping a drone in a stable hover.
- ***stabilization.ino*** the third POC, utilizing an *Adafruit Feather Sensor* equipped with an *Accelerometer/Gyroscope (LSM6DS3TRC)* and a *Magnetometer (LIS3MDL)* onboard. Combinging these sensors with the code from ***stabilization.c*** and only a few minor changes we are able to change thrust dependent on the angle of the board in real time.

## Demo
In this demo you can see the LED changing from green to red when the board is on a tilt at any angle. Green indicates that the board is level within a margin of error, and the Red indicates that it has gone outside of that margin.
> Adafruit Feather Sense nRF52840 running ***stabilization.ino***



https://github.com/user-attachments/assets/07dba06a-26e7-4e15-b6db-a0ad4bfe194a

