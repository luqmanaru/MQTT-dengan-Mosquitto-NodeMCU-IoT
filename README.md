# MQTT-dengan-Mosquitto-NodeMCU-IoT

![ESP8266](https://img.shields.io/badge/ESP8266-000000?style=flat&logo=espressif&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-3.1.1-orange)
![IoT](https://img.shields.io/badge/IoT-Project-blue)

Implementasi koneksi MQTT menggunakan Mosquitto dengan NodeMCU/ESP8266 untuk tugas individu mata kuliah Internet of Things 2.

## 🎯 Fitur Utama
- **Koneksi WiFi**: Menghubungkan NodeMCU ke jaringan WiFi
- **Koneksi MQTT**: Terhubung ke broker MQTT publik (test.mosquitto.org)
- **Publish Message**: Mengirim pesan ke topik MQTT setiap 2 detik
- **Subscribe Message**: Menerima pesan dari topik MQTT yang sama
- **Serial Monitor**: Menampilkan status koneksi dan pesan yang dikirim/diterima

## 🛠️ Komponen yang Dibutuhkan
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| NodeMCU ESP8266 | 1 | Mikrokontroler dengan WiFi built-in |
| Kabel USB | 1 | Untuk power dan programming |
| Komputer | 1 | Untuk programming dan Serial Monitor |
| Software Mosquitto | 1 | Untuk testing MQTT di CMD |

## 🔌 Skema Koneksi
| Komponen | Koneksi | Keterangan |
|----------|---------|------------|
| NodeMCU ESP8266 | USB ke Komputer | Untuk power dan komunikasi serial |
| NodeMCU ESP8266 | WiFi ke Router | Untuk koneksi internet |
| Komputer | Internet ke Broker MQTT | Untuk testing dengan Mosquitto |

**Catatan**: 
- Tidak ada komponen tambahan yang diperlukan untuk proyek ini
- NodeMCU sudah memiliki WiFi built-in dan dapat diprogram langsung melalui USB

## 📝 Kode Program
Kode program terdiri dari beberapa fungsi utama:

### Inklusi Library
```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
```

### Kredensial WiFi dan MQTT
```cpp
// WiFi dan MQTT
const char* ssid = "POCO F3";
const char* password = "yaudahpakeaja";
const char* mqtt_server = "test.mosquitto.org";
```

### Fungsi Setup WiFi
```cpp
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
```

### Fungsi Callback MQTT
```cpp
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
```

### Fungsi Reconnect MQTT
```cpp
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
      delay(2000);
    }
  }
}
```

### Fungsi Loop
```cpp
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
```

## 📊 Output Sistem
| Platform | Output | Deskripsi |
|----------|--------|-----------|
| Serial Monitor | `Connecting to [SSID]` | Mencoba terhubung ke WiFi |
| Serial Monitor | `........` | Menunggu koneksi WiFi terbentuk |
| Serial Monitor | `WiFi connected` | Koneksi WiFi berhasil |
| Serial Monitor | `IP address: 192.168.x.x` | Menampilkan IP address |
| Serial Monitor | `Attempting MQTT connection...` | Mencoba terhubung ke broker MQTT |
| Serial Monitor | `connected` | Koneksi MQTT berhasil |
| Serial Monitor | `Mengirim pesan: SAH KAN PERAMPASAN ASET` | Mengirim pesan ke topik |
| Serial Monitor | `Pesan berhasil dikirim.` | Konfirmasi pengiriman berhasil |
| CMD (Mosquitto) | `SAH KAN PERAMPASAN ASET` | Pesan diterima di broker |

## 🚀 Cara Penggunaan
1. **Install Mosquitto**: Download dan install software Mosquitto dari https://mosquitto.org/download/
2. **Setup Library**: Install board ESP8266 dan library PubSubClient di Arduino IDE
3. **Konfigurasi WiFi**: Ganti SSID dan password di kode dengan jaringan Anda
4. **Konfigurasi Topik**: Sesuaikan topik publish dan subscribe sesuai kebutuhan
5. **Upload Kode**: Upload kode ke NodeMCU
6. **Testing CMD**: Buka CMD sebagai administrator dan jalankan perintah subscribe:
   ```
   cd \
   cd program files\mosquitto
   mosquitto_sub -h test.mosquitto.org -t "iot2/kelompok2/hanif/aspirasi"
   ```

---
**luqmanaru**

Proyek ini dikembangkan sebagai tugas individu mata kuliah Internet of Things 2. Proyek ini mendemonstrasikan implementasi dasar protokol MQTT menggunakan NodeMCU dan broker Mosquitto.
