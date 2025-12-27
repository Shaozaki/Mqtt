#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// -- configurasi --
const char *SSID = "Nama Wifi";
const char *PASS = "Password wifi";
const char *MQTT_BROKER = "broker.hivemq.com";
const char *MQTT_ID = "ESP8266_Minimalist_001";
const int MQTT_PORT = 1883;

// -- Hardware --
#define PIN_DHT 14
#define PIN_RAIN A0
#define RAIN_DRY 750 
#define RAIN_WET 200 

// -- Objects --
DHT dht(PIN_DHT, DHT22);
Adafruit_BMP085 bmp;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

// -- Sensor Helpers --

float getTemp() { return dht.readTemperature(); }
float getHumid() { return dht.readHumidity(); }
float getPress()
{
  float p = bmp.readPressure();
  return (p == 0) ? 0 : p / 100.0F;
}


int getRainPct()
{
  int raw = analogRead(PIN_RAIN);
  return constrain(map(raw, RAIN_WET, RAIN_DRY, 100, 0), 0, 100);
}

String getRainStatus(int pct)
{
  if (pct <= 30)
    return "Kering ";
  if (pct <= 50)
    return "Rintik ";
  if (pct <= 80)
    return "Hujan Sedang ";
  return "Hujan Deras! ";
}

String getPressStatus(float p)
{
  return (p > 1013) ? "Tinggi" : (p >= 1000) ? "Sedang"
                                             : "Rendah";
}

// -- Connection Helpers --

void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect(MQTT_ID))
    {
      client.publish("station/cuaca/status", "Online");
    }
    else
    {
      delay(5000);
    }
  }
}

// 

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  bmp.begin();

  setupWiFi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

void loop()
{
  if (!client.connected())
    reconnect();
  client.loop();

//logika milis mengirim pesan setiao 2 detik
  if (millis() - lastMsg > 2000)
  {
    lastMsg = millis();

    float t = getTemp();
    float h = getHumid();
    float p = getPress();
    int r = getRainPct();

// serial monitor
    String stat = "Cuaca: " + getRainStatus(r) + " | Press: " + getPressStatus(p);

    Serial.print("Suhu: ");
    Serial.print(t);
    Serial.print(" | Lembab: ");
    Serial.print(h);
    Serial.print(" | Tekanan: ");
    Serial.print(p);
    Serial.print(" | Hujan: ");
    Serial.print(r);
    Serial.print("% | Status: ");
    Serial.println(stat);

    if (isnan(t) || isnan(h))
      return;

// mqtt_sub
    char buf[10];
    dtostrf(t, 1, 2, buf);
    client.publish("station/cuaca/suhu", buf);
    dtostrf(h, 1, 2, buf);
    client.publish("station/cuaca/lembab", buf);
    dtostrf(p, 1, 2, buf);
    client.publish("station/cuaca/tekanan", buf);
    itoa(r, buf, 10);
    client.publish("station/cuaca/hujan", buf);

     stat = "Cuaca: " + getRainStatus(r) + " | Press: " + getPressStatus(p);
    client.publish("station/cuaca/status", stat.c_str());

    // Indikator LED
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}