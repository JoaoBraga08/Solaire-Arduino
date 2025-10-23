// Inclusão das bibliotecas necessárias
#include <WiFi.h>       // Para conectar ao Wi-Fi
#include <HTTPClient.h>  // Para fazer requisições HTTP (POST)
#include <Servo.h>       // Para controlar o servo motor do rastreador solar

// --- CONFIGURAÇÕES DO PROJETO SOLAIRE ---
// Definição dos pinos dos sensores e atuadores
#define VOLTAGE_PIN 35    // Pino analógico para o sensor de tensão
#define CURRENT_PIN 36    // Pino analógico para o sensor de corrente
#define LDR_PIN     34    // Pino analógico para o sensor de luminosidade (LDR)
#define SERVO_PIN   13    // Pino digital para o servo motor

// --- CONSTANTES DE CALIBRAÇÃO (AJUSTE CONFORME SEUS SENSORES) ---
// Exemplo para um divisor de tensão. (R1+R2)/R2. Precisa ser calibrado.
const float VOLTAGE_DIVIDER_RATIO = 11.0; 
// Exemplo para um sensor de corrente ACS712. Precisa ser calibrado.
const float SENSOR_SENSITIVITY = 0.100; // Sensibilidade em V/A (ex: 100mV/A)
const float SENSOR_OFFSET = 2.5; // Tensão de offset do sensor (geralmente VCC/2)

// --- DADOS DA REDE E SERVIDOR ---
// Altere com os dados da sua rede
const char* ssid = "Ana_julia"; // Nome da sua rede Wi-Fi
const char* password = "123456789";   // Senha da sua rede Wi-Fi

// URL do seu servidor back-end. 
// IMPORTANTE: Use o IP do computador onde o 'server.js' está rodando.  
const char* serverName = "http://10.92.199.29:3000/data";

// Inicializa o objeto do servo motor
Servo solarServo;

void setup() {
  // Inicia a comunicação serial para depuração
  Serial.begin(9600);
  
  // Anexa o servo ao pino definido e o posiciona no ângulo inicial (90 graus)
  solarServo.attach(SERVO_PIN);
  solarServo.write(90);

  // Inicia a conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi...");
  
  // Loop que aguarda a conexão ser estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // --- LEITURA DOS SENSORES DA PLACA SOLAR ---
    
    // Lê o valor bruto do sensor de tensão e converte para Volts
    int rawVoltage = analogRead(VOLTAGE_PIN);
    float voltage = (rawVoltage * (3.3 / 4095.0)) * VOLTAGE_DIVIDER_RATIO;
    
    // Lê o valor bruto do sensor de corrente e converte para Amperes
    int rawCurrent = analogRead(CURRENT_PIN);
    float current = ((rawCurrent * (3.3 / 4095.0)) - SENSOR_OFFSET) / SENSOR_SENSITIVITY;
    if (current < 0) current = 0; // Corrige pequenas flutuações negativas

    // Calcula a potência em Watts
    float power = voltage * current;

    // Lê o valor do LDR para intensidade de luz
    int lightValue = analogRead(LDR_PIN);

    // --- LÓGICA DE CONTROLE (Exemplo simples de rastreamento) ---
    // Esta é uma lógica de exemplo. Um sistema real usaria múltiplos LDRs
    // para determinar a posição do sol e ajustar o servo.
    int servoAngle = map(lightValue, 0, 4095, 0, 180); // Mapeia a luz para o ângulo do servo
    solarServo.write(servoAngle);

    // --- ENVIO DOS DADOS PARA O SERVIDOR ---
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Monta a string JSON com os novos dados
    String payload = "{\"voltage\":" + String(voltage) +
                     ",\"current\":" + String(current) +
                     ",\"power\":" + String(power) +
                     ",\"light\":" + String(lightValue) +
                     ",\"servoAngle\":" + String(servoAngle) + "}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("--------------------");
      Serial.println("Dados do Solaire enviados com sucesso!");
      Serial.println("Tensão: " + String(voltage) + " V");
      Serial.println("Corrente: " + String(current) + " A");
      Serial.println("Potência: " + String(power) + " W");
      Serial.println("Luminosidade: " + String(lightValue));
      Serial.println("Ângulo do Servo: " + String(servoAngle) + "°");
    } else {
      Serial.print("Erro ao enviar dados. Código: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("Conexão Wi-Fi perdida.");
  }
  
  // Aguarda 5 segundos antes da próxima leitura e envio
  delay(5000);
}