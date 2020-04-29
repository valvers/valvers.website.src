// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

// Modified by Brian Sidebotham <brian.sidebotham@gmail.com>
// Changes:
// - Allow connectivity to AWS IoT as part of the aws-iot-esp32-mqtt-tutorial

#include <WiFiClientSecure.h>
#include <MQTT.h>

#include "wifi-credentials.h"
#include "aws-credentials.h"

const char wifi_ssid[] = WIFI_SSID;
const char wifi_password[] = WIFI_PASSWORD;

const char aws_iot_endpoint[] = AWS_IOT_ENDPOINT;
const int aws_iot_port = 8883;
const char aws_thing_name[] = "ESP32Example";

const char* AWS_ROOT_CA = R"RAWTEXT(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)RAWTEXT";


WiFiClientSecure net;
MQTTClient client(512);

unsigned long lastMillis = 0;

void handleTLSError() {
  char errorBuffer[100];

  if( net.lastError(errorBuffer, sizeof(errorBuffer)) != 0) {
    Serial.print("TLS Error: ");
    Serial.println(errorBuffer);
  }
}

void handleError() {
  Serial.print("MQTT Client Error: ");
  Serial.println(client.lastError());

  handleTLSError();
}

void connect() {


  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println("connecting...");
  while (!client.connect(aws_thing_name)) {
    handleError();
    delay(1000);
  }

  Serial.println("connected!");

  Serial.println("Subscribing to " AWS_IOT_TOPIC_SHADOW_UPDATE);

  if( !client.subscribe(AWS_IOT_TOPIC_SHADOW_UPDATE, 1) ) {
    Serial.println("Failed to subscribe to " AWS_IOT_TOPIC_SHADOW_UPDATE);
  }

  Serial.println("Subscribing to " AWS_IOT_TOPIC_SHADOW_UPDATE_ACCEPTED);

  if( !client.subscribe(AWS_IOT_TOPIC_SHADOW_UPDATE_ACCEPTED, 1) ) {
    Serial.println("Failed to subscribe to " AWS_IOT_TOPIC_SHADOW_UPDATE_ACCEPTED);
  }
  
  Serial.println("Subscribing to " AWS_IOT_TOPIC_SHADOW_UPDATE_REJECTED);

  if( !client.subscribe(AWS_IOT_TOPIC_SHADOW_UPDATE_REJECTED, 1) ) {
    Serial.println("Failed to subscribe to " AWS_IOT_TOPIC_SHADOW_UPDATE_REJECTED);
  }

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

  // Setup the certificates for MQTT connection auth
  Serial.println("Setting AWS Root CA");
  net.setCACert(AWS_ROOT_CA);
  handleTLSError();

  Serial.println("Setting Thing Certificate");
  net.setCertificate(AWS_THING_CERT);
  handleTLSError();

  Serial.println("Setting the Thing Private Key");
  net.setPrivateKey(AWS_PRIVATE_KEY);
  handleTLSError();

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  //
  // MQTT brokers usually use port 8883 for secure connections.
  Serial.printf("Beginning MQTT Client %s:%u\n", aws_iot_endpoint, aws_iot_port);
  client.begin(aws_iot_endpoint, aws_iot_port, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  // publish a message roughly every second.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    if (!client.connected()) {
      handleError();
      connect();
    }
    Serial.println("Publishing State...");
    if( !client.publish(AWS_IOT_TOPIC_SHADOW_UPDATE, R"JSON({
        "state": {
              "reported": {
                  "color": "red",
                  "power": "on"
              }
        }
    }
    )JSON", false, 1)) {
        Serial.printf("Failed to publish state\n");
        handleError();
    }
  }
}
