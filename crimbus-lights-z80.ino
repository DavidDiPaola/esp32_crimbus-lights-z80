/* 2019 David DiPaola, licensed CC0 (public domain worldwide) */

extern "C" {
#include "src/z80/z80.h"
}

#include <Adafruit_NeoPixel.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include "src/wificred.h"

const int config_net_TIMEOUT = 1000;
#define LED_BUILTIN 2
#define pixels_PIN 15
#define pixels_LEDCOUNT 50

z80 cpu;
#define memory_LENGTH 0x10000
uint8_t memory[memory_LENGTH];
#define io_LENGTH 0x100
uint8_t io[io_LENGTH];
Adafruit_NeoPixel pixels;
WiFiMulti wifim;
WiFiServer server(8080);

static void pixels_update(uint8_t port) {
  if (port >= (pixels_LEDCOUNT * 3)) {
    return;
  }

  int pixel = port / 3;
  int io_i = pixel * 3;
  int r = io[io_i+0];
  int g = io[io_i+1];
  int b = io[io_i+2];
  pixels.setPixelColor(pixel, pixels.Color(r, g, b));
  pixels.show();
}

static uint8_t memory_read(void * userdata, uint16_t addr) {
  return memory[addr];
}

static void memory_write(void * userdata, uint16_t addr, uint8_t val) {
  memory[addr] = val;
}

static uint8_t io_read(z80 * z, uint8_t port) {
  return io[port];
}

static void io_write(z80 * z, uint8_t port, uint8_t val) {
  io[port] = val;
  pixels_update(port);
}

static void cpu_init() {
  for (size_t i=0; i<io_LENGTH; i++) {
    io[i] = 0;
  }

  z80_init(&cpu);
  cpu.read_byte = memory_read;
  cpu.write_byte = memory_write;
  cpu.port_in = io_read;
  cpu.port_out = io_write;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  delay(10);

  Serial.println("init:");

  Serial.print("\t" "CPU... ");
  cpu_init();
  Serial.println("done");

  Serial.print("\t" "LEDs... ");
  pixels = Adafruit_NeoPixel(
    pixels_LEDCOUNT,
    pixels_PIN,
    NEO_GRB + NEO_KHZ800
  );
  pixels.begin();
  pixels.clear();
  pixels.show();
  Serial.println("done");

  Serial.print("\t" "WiFi...");
  wifim.addAP(wificred_SSID, wificred_PASSWORD);
  while (wifim.run() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.print(" done (IP: ");
  Serial.print(WiFi.localIP());
  Serial.print(", MAC: ");
  Serial.print(WiFi.macAddress());
  Serial.println(")");
  delay(500);

  Serial.print("\t" "server... ");
  server.begin();
  Serial.println("done");

  Serial.println("init done");
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  static bool memory_loaded = false;
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("client connected");

    memory_loaded = false;
    size_t i = 0;
    while (client.connected()) {
      int t = 0;
      while (!client.available()) {
        delay(1);
        t++;
        if (t >= config_net_TIMEOUT) {
          goto done;
        }
      }
      uint8_t b = client.read();
      memory[i++] = b;
      if (i >= memory_LENGTH) {
        break;
      }
    }

    done:
    cpu_init();
    memory_loaded = true;
    Serial.println("data received");
    client.stop();
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (memory_loaded) {
    z80_step(&cpu);
  }
}
