#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi dan MQTT
const char* ssid = "POCO F3";
const char* password = "yaudahpakeaja";
const char* mqtt_server = "test.mosquitto.org";

// Objek koneksi
WiFiClient espClient;
PubSubClient client(espClient);

// Variabel pesan dan interval
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
unsigned long lastMsg = 0;
const long interval = 2000; // kirim setiap 2 detik

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);
  } else if ((char)payload[0] == '0') {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("iot2/kelompok2/hanif/aspirasi", "Testing IOT 2");
      client.subscribe("iot2/kelompok2/hanif/aspirasi");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000); // pokoknya berubah 2 detik
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    String message = "SAH KAN PERAMPASAN ASET";
    Serial.print("Mengirim pesan: ");
    Serial.println(message);
    if (client.publish("iot2/kelompok2/hanif/aspirasi", message.c_str())) {
      Serial.println("Pesan berhasil dikirim.");
    } else {
      Serial.println("Gagal mengirim pesan.");
    }
  }
}
