Xbox controller for Amiga

Introduction
This is a small code based on the usb host shild exampe for xbox controller to be able to control the amiga with xbox controller.
I'm open to any PR's or comments.


v.0.0.1
The code is very much hacked together. But it's working. mostly.

---

What you will need:
- Arduino uno
- USB Host shield
- Cables
- DB9 connector
- Power source for arduino

USB HOST SHIELD LIBRARY:
https://github.com/felis/USB_Host_Shield_2.0
I also commit it to this repo as a zip, just in case.

DB9 connector:
To connect the DB9 to pins, follow the setPinToJoy function.
The reason for the non-linear pin usage on arduino is because the USB host shield uses some pins.

---

The controls are the following:

X - Toggles autofire on/off
A - Fire 1 / Left click
B - Fire 2 / Right click
Xbox button - Toggles between mouse and joy mode. Joy is default

Left Analog - Function as the arrows. For the mouse, it's a bit buggy when used diagonally, but it controls the speed of the mouse as well.
Arrows - Function as expected for mouse and joy mode. Mouse is a fixed speed, and bit fast for some usecase.
