Raspberry Pi NES controller
===========================

This an implementation of the low-level protocol used by NES controllers. The program can be used on a Raspberry Pi. uinput is used to map the state of the buttons to linux as an input device.

The program uses the BCM2835 driver (http://www.airspayce.com/mikem/bcm2835/) for reading/writing the general purposes I/O pins (GPIO).

Installation:
* Download GCM2835 and install (./configure && make && sudo make install)
* Compile NES controller program: make
* Upload binary to Raspberry Pi and start it: ./bin/rpi-nc

Other:
Raspberry Pi Hardware revision: 2

Used GPIO pins:
P1-14	Ground
P1-15	GPIO22, Latch (output)
P1-16	GPIO23, Pulse (output)
P1-17	3.3V
P1-18	GPIO24, Data (input)
