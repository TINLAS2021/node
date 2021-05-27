#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "globals.h"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);

  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting...");
    delay(300);
  }

  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, LOW);

  randomSeed(micros());
}

void moveMotor(int16_t val) {
  ax12a.move(ID, initial_pos + val);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char buff[length];
  memcpy(buff, payload, length);
  pos = constrain(atoi(buff), min_val, max_val);
//  pos = atoi(buff);
  moveMotor(pos);
}

void reconnect() {
  digitalWrite(LED_BUILTIN, HIGH);
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    String clientId = "SnakeClient-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      // Once connected, publish an announcement...
      client.publish(OUT_TOPIC, "Reconnected with MQTT...");
      // ... and resubscribe
      client.subscribe(IN_TOPIC);
    } else {
      delay(1000);
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(1000000ul);
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  pinMode(BUILTIN_LED, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    itoa(pos, msg, 10);
    client.publish(OUT_TOPIC, msg);
  }
}
