#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <c_types.h>

/* Initialize variables */
const char *ssid = "HoverCraft";
const char *password = "aapaapaap";

WiFiServer server(1337);
WiFiClient client;

byte fans = 0;
time_t time1 = 0;
/*---------------------*/

ICACHE_FLASH_ATTR
void setup() {
  // Set gpio pins
  pinMode(0, OUTPUT); // GPIO 0 - Lift fan
  pinMode(1, OUTPUT); // TX - Accelerate fan
  pinMode(2, OUTPUT); // GPIO 2 - Left fan
  pinMode(3, OUTPUT); // RX - Right fan

  // Start access point
  WiFi.softAP(ssid, password);

  // Start access point
  server.begin();
}

ICACHE_RAM_ATTR
void loop() {
  // Accept client
  client = server.available();
  while (client.connected()) {
    delay(10);

    // When there is no data received in 15 seconds stop the fans
    if ((millis() - time1) >= 15000) {
      stopFans();
    }

    if (client.available() > 0) {
      // Update time
      time1 = millis();

      switch (client.read()) {
        // Lift fan
        case '0':
          fans ^= 1;
          digitalWrite(0, ((fans & 1) == 1 ? 1 : 0));
          break;

        // Accelerate fan
        case '1':
          fans ^= 2;
          digitalWrite(1, ((fans & 2) == 2 ? 1 : 0));
          break;

        // Left fan
        case '2':
          fans ^= 4;
          digitalWrite(2, ((fans & 4) == 4 ? 1 : 0));
          break;

        // Right fan
        case '3':
          fans ^= 8;
          digitalWrite(3, ((fans & 8) == 8 ? 1 : 0));
          break;
      }
    }
  }
  // When connection is lost
  stopFans();
}

ICACHE_FLASH_ATTR
void stopFans()
{
  fans = 0;
  digitalWrite(0, 0);
  digitalWrite(1, 0);
  digitalWrite(2, 0);
  digitalWrite(3, 0);
}

