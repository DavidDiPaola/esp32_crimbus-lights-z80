# crimbus-lights-z80
A crimbus tree controlled by an emulated Z80.

## Installation
  1. Wire up a NeoPixel compatible LED strand to `D15` on an ESP32.
  2. Update `config.h`'s `config_ledstrip_LEDCOUNT` to match the length of the LED strand.
  3. Update `config.h`'s `config_wifi_SSID` and `config_wifi_PASSWORD` appropriately.
  4. Build, upload code to ESP32.
  5. (optional) Set up static DHCP so the ESP32 will always get the same IP address. (The MAC address will be printed on the serial console)
  6. Blast a raw Z80 binary to the ESP32's port 8080.

## Writing Z80 code
  1. Install `binutils-z80` or similar.
  2. Edit `test.s` as you wish.
  3. Open a terminal, `cd` into this project's directory, `cd z80-code/`
  4. `make send`

## Notes
The virtual Z80's IO space is mapped as 1 byte per color channel per LED. ie: IO port 0 is the first LED's first color channel, IO port 2 is the first LED's third color channel, IO port 3 is the second LED's first color channel, etc.

The [Z80 emulator](https://github.com/superzazu/z80) was written by [superzazu](https://github.com/superzazu) and is under the MIT license. This code is under `src/z80/`.

My code is licensed CC0.

