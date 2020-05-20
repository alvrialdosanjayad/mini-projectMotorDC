// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "ssid";
const char pass[] = "pass";
String pesan = "";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ESP32", "key(Username)", "secret(Password")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("MotorDC_1");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "MotorDC_1") {
    if(payload=="nyala") {
      digitalWrite(23,LOW);
      pesan = "Nyala";
    }else if (payload == "mati") {
      digitalWrite(23,HIGH);
      pesan = "Mati";
    }
  }

  if(pesan == "Nyala") {
    client.publish("infoMotorDC",pesan);
  } else if(pesan == "Mati") {
    client.publish("infoMotorDC",pesan);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  pinMode(23,OUTPUT);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}
