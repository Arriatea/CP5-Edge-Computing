#  Checkpoint 5 Edge-Computing – Leitura de DHT22 e LDR via MQTT

##  Descrição do Projeto
Este projeto realiza a leitura dos sensores DHT22 (temperatura e umidade) e LDR (luminosidade) utilizando o ESP32, com envio dos dados para um servidor MQTT.  
A comunicação é feita de forma simples e direta, o dispositivo apenas conecta, coleta os dados e publica no servidor MQTT configurado (porta 69).

---

## Objetivo
- Demonstrar a captura de dados dos sensores **DHT22** e **LDR** via ESP32.  
- Enviar os dados coletados via protocolo **MQTT**.  
- Permitir visualização no aplicativo **MyMQTT** ou outro cliente MQTT compatível.  

---

## Componentes Utilizados
| Componente | Quantidade | Função |
|-------------|-------------|--------|
| ESP32 | 1 | Microcontrolador principal |
| DHT22 | 1 | Sensor de temperatura e umidade |
| Módulo LDR | 1 | Sensor de luminosidade analógico |
| Jumpers | Vários | Ligações entre os componentes |

---

## Esquemático de Ligações

### 🧾 DHT22
| Pino DHT22 | Conexão ESP32 |
|-------------|----------------|
| VCC | 3.3V |
| SDA | GPIO 23 |
| GND | GND |

### 💡 LDR (Módulo)
| Pino Módulo LDR | Conexão ESP32 |
|-----------------|----------------|
| AO (Analógico) | GPIO 13 |
| VCC | 5V |
| GND | GND |

---

## 💻 Código-Fonte (ESP32)

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT22
#define LDR_PIN 13

// Configurações Wi-Fi
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

// Configurações MQTT
const char* mqtt_server = "IP_DO_SERVIDOR";
const int mqtt_port = 69; // Porta
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
