/* 2019 David DiPaola, licensed CC0 (public domain worldwide) */

extern "C" {
#include "src/z80/z80.h"
}

#include <Adafruit_NeoPixel.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include "config.h"

#define LED_BUILTIN 2

z80 cpu;
#define memory_LENGTH 0x10000
uint8_t memory[memory_LENGTH];
#define io_LENGTH 0x100
uint8_t io[io_LENGTH];
Adafruit_NeoPixel ledstrip;
WiFiMulti wifim;
WiFiServer server(config_net_PORT);

static void ledstrip_update(uint8_t port) {
  if (port >= (config_ledstrip_LEDCOUNT * 3)) {
    return;
  }

  int pixel = port / 3;
  int io_i = pixel * 3;
  int r = io[io_i+0];
  int g = io[io_i+1];
  int b = io[io_i+2];
  ledstrip.setPixelColor(pixel, ledstrip.Color(r, g, b));
  ledstrip.show();
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
  ledstrip_update(port);
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
  ledstrip = Adafruit_NeoPixel(
    config_ledstrip_LEDCOUNT,
    config_ledstrip_PIN,
    NEO_GRB + NEO_KHZ800
  );
  ledstrip.begin();
  ledstrip.clear();
  ledstrip.show();
  Serial.println("done");

  Serial.print("\t" "WiFi...");
  wifim.addAP(config_wifi_SSID, config_wifi_PASSWORD);
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
