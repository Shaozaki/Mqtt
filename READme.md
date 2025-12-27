# 🌦️ Stasiun Cuaca Mini (IoT ESP8266)

Halo! Ini adalah proyek **Stasiun Cuaca Pintar** yang gampang banget dibuat.
Intinya, alat ini bakal baca suhu, kelembaban, dan hujan, terus datanya dikirim ke laptopmu lewat internet (WiFi). Keren kan? 😎

## 🛠️ Alat yang Dibutuhkan
Siapin dulu bahan-bahannya ya:
1.  **ESP8266 (NodeMCU/Wemos)** - Ini otak utamanya.
2.  **Sensor DHT22** - Buat ukur suhu & lembab.
3.  **Sensor BMP180** - Buat ukur tekanan udara.
4.  **Sensor Hujan (Rain Module)** - Biar tau lagi hujan atau kering.
5.  **Kabel Jumper & Breadboard** - Buat nyambung-nyambungin kabel.

## 🔌 Cara Sambung Kabel (Wiring)
Jangan sampai salah colok ya, nanti nggak nyala lho!

* **Sensor DHT22**
    * Pin Data ➡️ Pin **D5** (GPIO 14) di ESP8266
    * VCC ➡️ 3V
    * GND ➡️ G
* **Sensor Hujan (Analog)**
    * Pin A0 ➡️ Pin **A0** di ESP8266
    * VCC    ➡️ 3V
    * GND    ➡️ G
* **Sensor BMP180**
    * SDA ➡️ Pin **D2** (GPIO 4)
    * SCL ➡️ Pin **D1** (GPIO 5)
    * VCC ➡️ 3V
    * GND ➡️ G

## 💻 Persiapan di Laptop (Arduino IDE)
Sebelum upload kode, install dulu "kamus" (Library) biar kodenya jalan:

1. Buka Arduino IDE, klik menu **Sketch** > **Include Library** > **Manage Libraries**.
2. Cari dan install 3 library ini:
   - `PubSubClient` (buat kirim data ke internet/MQTT).
   - `DHT sensor library` (buat sensor suhu).
   - `Adafruit BMP085 Library` (buat sensor tekanan).

## 🚀 Cara Jalaninnya

1.  **Edit Kodingan:**
    Cari bagian ini di kodingan, lalu isi sama WiFi rumah/kosan kamu:
    ```cpp
    const char *SSID = "Nama Wifi Kamu";
    const char *PASS = "Password Wifi Kamu";
    ```
2.  **Upload:** Colok ESP8266 ke laptop, pilih *Port* yang sesuai, lalu klik tombol panah kanan (➡️) buat upload.
3.  **Tunggu:** Kalau sudah selesai upload, buka **Serial Monitor** (set ke 115200 baud). Nanti muncul tulisan suhu dan status cuaca tiap 2 detik.

## 📡 Cara Pantau Lewat PowerShell
Biar berasa kayak *hacker*, kamu bisa lihat datanya lewat terminal Windows:

1. Pastikan kamu sudah install **Mosquitto**.
2. Buka PowerShell, ketik perintah ini:
   ```powershell
   mosquitto_sub -h broker.hivemq.com -t "station/cuaca/#" -v
  
 Boom! 💥 Data cuaca dari alatmu bakal muncul terus di layar hitam itu.

## 💡 Catatan

    Kalau alatnya kedip-kedip LED-nya, berarti dia lagi kirim data. Aman!

    Kalau di Serial Monitor tulisannya aneh (kotak-kotak), ganti angka di pojok kanan bawah jadi 115200 baud.

