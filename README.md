# crimbus-lights-z80
A crimbus tree controlled by an emulated Z80.

## Installation
  1. Wire up a NeoPixel compatible LED strand to `D15` on an ESP32.
  2. Edit the `.ino` file's `pixels_LEDCOUNT` to match the length of the LED strand.
  3. Put WiFi credentials into `src/wificred.h`.
  4. Build, upload code to ESP32.
  5. (optional) Set up static DHCP so the ESP32 will always get the same IP address.
  6. Blast a 64KB image containing Z80 code to the ESP32's port 8080 (TCP).

## Notes
The [Z80 emulator](https://github.com/superzazu/z80) was written by [superzazu](https://github.com/superzazu) and is under the MIT license. This code is under `src/z80/`.

My code is licensed CC0.

