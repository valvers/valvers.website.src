// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFiClientSecure.h>
#include <MQTT.h>

#include "wifi-credentials.h"

const char wifi_ssid[] = WIFI_SSID;
const char wifi_password[] = WIFI_PASSWORD;

WiFiClientSecure wifi;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("\nconnecting...");
    while (!client.connect("bsidebotham", "try", "try")) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nconnected!");
  
    client.publish("/hello", "", true);
    client.subscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
    Serial.println("incoming: " + topic + " - " + payload);

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(wifi_ssid, wifi_password);

    // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
    // by Arduino. You need to set the IP address directly.
    //
    // MQTT brokers usually use port 8883 for secure connections.
    client.begin("broker.shiftr.io", 8883, wifi);
    client.onMessage(messageReceived);

    connect();
}

void loop() {
    client.loop();
    delay(10);  // <- fixes some issues with WiFi stability

    // publish a message roughly every second.
    if (millis() - lastMillis > 1000) {
        if (!client.connected()) {
            Serial.print("lastError: ");
            Serial.println(client.lastError());
            connect();
        }
    
        lastMillis = millis();
        client.publish("/hello", "world");
  }
}
