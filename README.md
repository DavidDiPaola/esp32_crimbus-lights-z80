# crimbus-lights-z80
A crimbus tree controlled by an emulated Z80.

## Installation
  1. Wire up a NeoPixel compatible LED strand to `D15` on an ESP32.
  2. Edit the `config.h`:
    1. Update `config_ledstrip_LEDCOUNT` to match the length of the LED strand.
    2. Update `config_wifi_SSID` and `config_wifi_PASSWORD` appropriately.
  3. Build, upload code to ESP32.
  4. (optional) Set up static DHCP so the ESP32 will always get the same IP address. (The MAC address will be printed on the serial console)
  5. Blast a raw Z80 binary to the ESP32's port 8080.
    1. Install `binutils-z80` or similar.
    2. Open a terminal, `cd` into this project's directory.
    3. `cd ./z80-code/`
    4. `make send`
    5. (Edit `test.s` if you want to change the Z80 code)

## Notes
The [Z80 emulator](https://github.com/superzazu/z80) was written by [superzazu](https://github.com/superzazu) and is under the MIT license. This code is under `src/z80/`.

My code is licensed CC0.

