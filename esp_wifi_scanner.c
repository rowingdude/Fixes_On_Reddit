#include "WiFi.h"

#define MAX_NETWORKS 50
#define MAX_RSSI 128

uint8_t unique_rssi[(MAX_RSSI + 7) / 8]; 
uint8_t rssi_count = 0;

bool is_rssi_unique(int rssi) {
  int index = rssi / 8;
  int bit = rssi % 8;
  return !(unique_rssi[index] & (1 << bit));
}

void add_rssi_to_set(int rssi) {
  int index = rssi / 8;
  int bit = rssi % 8;
  unique_rssi[index] |= (1 << bit);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.println("SSID | RSSI");
  }

  rssi_count = 0;
  memset(unique_rssi, 0, sizeof(unique_rssi));

  for (int i = 0; i < n; ++i) {
    // Check if the RSSI is unique
    int rssi = WiFi.RSSI(i);
    if (is_rssi_unique(rssi)) {
      // Add the unique RSSI to the list and print it
      add_rssi_to_set(rssi);
      rssi_count++;
      Serial.print(WiFi.SSID(i).c_str());
      Serial.print(" , ");
      Serial.print(rssi);
      Serial.println();
    }
  }
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}
