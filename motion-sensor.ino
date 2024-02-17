#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

const char* ssid = "";
const char* password = "";

const char* host = "192.168.0.59";
const char* username = "";
const int sensorID = 1;

#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID ""
#define WIFI_AUTH ""

// data from DHCP: IP address, network mask, Gateway, DNS
#define DATA_IPADDR IPAddress(192, 168, 0, 138)
#define DATA_IPMASK IPAddress(255, 255, 255, 0)
#define DATA_GATEWY IPAddress(192, 168, 0, 1)
#define DATA_DNS1 IPAddress()

// data from your Wifi connect: Channel, BSSID
#define DATA_WIFI_CH 11
#define DATA_WIFI_BSSID \
  { 0xB8, 0x66, 0x85, 0xF0, 0xD7, 0x35 }

#endif
int wifi_fast() {

  WiFi.persistent(true);
  WiFi.config(DATA_IPADDR, DATA_GATEWY, DATA_IPMASK);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("ESP Test");
  uint8_t my_bssid[] = DATA_WIFI_BSSID;
  char my_ssid[] = WIFI_SSID;
  char my_auth[] = WIFI_AUTH;
  WiFi.begin(&my_ssid[0], &my_auth[0], DATA_WIFI_CH, &my_bssid[0], true);

  WiFi.persistent(true);
  uint32_t timeout = millis() + 5000;
  while ((WiFi.status() != WL_CONNECTED) && (millis() < timeout)) { delay(5); }
  return (WiFi.status() == WL_CONNECTED);
}

const int digitalInputPin = D2;
struct {
  uint32_t crc32;
  bool previousStatus;
} rtcData;

void setup() {
  // Serial.begin(115200);
  pinMode(digitalInputPin, INPUT_PULLUP);
  // ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, sizeof(rtcData));
  // TODO: Setup WIFI data
}


// int getWiFiChannel(int freq) {
//   if (freq >= 2412 && freq <= 2484) {
//     return (freq - 2412) / 5 + 1;
//   } else if (freq >= 5180 && freq <= 5825) {
//     return (freq - 5180) / 5 + 36;
//   } else {
//     return 0; // Unable to determine WiFi channel
//   }
// }

// void setup() {
//   Serial.begin(115200);
// Connect to WiFi network
// Serial.println();
// Serial.println("Connecting to WiFi...");


// Serial.println("");
// Serial.println("WiFi connected");
// Serial.print("IP address: ");
// Serial.println(WiFi.localIP());
// Serial.print("Subnet mask: ");
// Serial.println(WiFi.subnetMask());

// Serial.print("Gateway address: ");
// Serial.println(WiFi.gatewayIP());

// Serial.print("DNS server address: ");
// Serial.println(WiFi.dnsIP());

// Serial.print("WiFi BSSID: ");
// Serial.println(WiFi.BSSIDstr());

//   Serial.print("Connection time: ");
// Serial.print(endTime - startTime);
// Serial.println(" milliseconds");
// WiFi.setAutoConnect(false);
//   wifi_fast();
// }


void sendPresenceRequest(bool presenceState) {
  WiFiClient client;  // Create WiFiClient object

  HTTPClient http;

  if (http.begin(client, "http://192.168.0.59/api/MrvvPyd2rFxW03L6dBmNMgC1HQgwh-tEfEGta5HR/sensors/18/state")) {  // Specify the target URL
    http.addHeader("Content-Type", "application/json");                                                           // Set content type
    http.addHeader("hue-application-key", "CD885EFEC9B8B400F99D3C97EBCB1486");                                    // Add Hue application key

    // Create JSON payload
    String payload = "{\"presence\": ";
    payload += presenceState ? "true" : "false";
    payload += "}";

    int httpResponseCode = http.PUT(payload);  // Send the request with the JSON payload

    if (httpResponseCode > 0) {
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);
    } else {
      // Serial.print("Error code: ");

      String error = http.errorToString(httpResponseCode);
      // Serial.println("Error: " + error);
      // Serial.println(httpResponseCode);
    }

    http.end();  // Free resources
  } else {
    // Serial.println("Unable to connect to the server");
  }
}
void sendStatusToBridge(bool state) {
  // unsigned long startTime = millis();

  wifi_fast();

  // unsigned long endTime = millis();

  // Serial.println(state);
  // Serial.println("Connection time 2: ");
  // Serial.println(state);
  // Serial.print(endTime - startTime);
  // Serial.println(" milliseconds");
  sendPresenceRequest(state);
}

void loop() {
  ESP.rtcUserMemoryRead(0, (uint32_t*)&rtcData, sizeof(rtcData));
  bool previousStatus = rtcData.previousStatus;
  // Serial.print("..");
  int sensorValue = digitalRead(digitalInputPin);  // Read the state of the digital input pin
  if (previousStatus != sensorValue) {
    sendStatusToBridge(sensorValue == HIGH);
    previousStatus = sensorValue;
    rtcData.previousStatus = sensorValue == HIGH;
    ESP.rtcUserMemoryWrite(0, (uint32_t*)&rtcData, sizeof(rtcData));
  }

  ESP.deepSleep(300e3);
  // delay(300);
  // delay(200);
  // if (sensorValue == HIGH) {
  //   // Serial.println("Digital input is HIGH");
  //   if(highCount >= 4) {
  //     sendStatusToBridge(true);
  //   } else {
  //     highCount ++;
  //   }
  //   count = 0;

  //   delay(100);
  // } else {
  //   highCount = 0;

  //   // Serial.println("Digital input is LOW");
  //   if(count >= 100) {
  //     sendStatusToBridge(false);
  //   } else {
  //     count ++;
  //   }

  //   delay(100);
  // }
}
