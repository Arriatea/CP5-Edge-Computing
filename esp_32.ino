#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define LDR_PIN 34

// Configurações Wi-Fi
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

// Configurações MQTT
const char* mqtt_server = "IP_DO_SERVIDOR_DO_PROFESSOR";
const int mqtt_port = 69;
const char* topicTemp = "sensor/temperatura";
const char* topicUmid = "sensor/umidade";
const char* topicLuz = "sensor/luminosidade";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);
  conectarMQTT();
}

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int luz = analogRead(LDR_PIN);

  if (!isnan(temperatura) && !isnan(umidade)) {
    client.publish(topicTemp, String(temperatura).c_str());
    client.publish(topicUmid, String(umidade).c_str());
    client.publish(topicLuz, String(luz).c_str());
  }

  delay(5000); // Envia a cada 5 segundos
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 5000) {
    delay(500);
  }
}

void conectarMQTT() {
  unsigned long inicio = millis();
  while (!client.connected() && millis() - inicio < 5000) {
    client.connect("ESP32_Client");
    delay(500);
  }
}