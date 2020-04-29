#include <WiFiClientSecure.h>

#include "wifi-credentials.h"

const char wifi_ssid[] = WIFI_SSID;
const char wifi_password[] = WIFI_PASSWORD;

WiFiClientSecure wifi;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.print("Connecting to SSID: ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_password);

    while( WiFi.status() != WL_CONNECTED ) {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.print(":");
    delay(10000);
}
