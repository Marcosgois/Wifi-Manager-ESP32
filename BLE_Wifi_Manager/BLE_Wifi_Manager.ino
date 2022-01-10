#include "SimpleBLE.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#include "WiFi.h"

SimpleBLE ble;

void onButton() {
  String out = "BLE32 name: ";
  out += String(millis() / 1000);
  Serial.println(out);
  ble.begin(out);
}


void wifiScan() {
  String out = "";
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    out = "no networks found";
    Serial.println("no networks found");
  } else {
    out += String(n);
    out += " networks found\n";
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
      out += String(i+1);
      out += ":";
      out += String(WiFi.SSID(i));
      out += " (";
      out += String(WiFi.RSSI(i));
      out += ")";
      (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? out += "\n" : out += "*\n";
    }
  }
  Serial.println("");
  Serial.println(out);
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(0, INPUT_PULLUP);
  Serial.print("ESP32 SDK: ");
  Serial.println(ESP.getSdkVersion());
  ble.begin("Reuk Tec IOT");
  Serial.println("Press the button to scan the WIFI's name");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop()
{
  static uint8_t lastPinState = 1;
  uint8_t pinState = digitalRead(0);
  if (!pinState && lastPinState) {
    wifiScan();
  }
  lastPinState = pinState;
  while (Serial.available()) Serial.write(Serial.read());
}
