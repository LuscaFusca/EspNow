#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xB8, 0xD6, 0x1A, 0x41, 0xF8, 0x58};

const int potPinA = 34; // Pino do potenciômetro
int lastPotValueA = 0; // Valor do potenciômetro da última leitura
const int potPinB = 35; // Pino do potenciômetro
int lastPotValueB = 0; // Valor do potenciômetro da última leitura
const int potPinC = 32; // Pino do potenciômetro
int lastPotValueC = 0; // Valor do potenciômetro da última leitura
const int potPinD = 33; // Pino do potenciômetro
int lastPotValueD = 0; // Valor do potenciômetro da última leitura
const int potPinE = 36; // Pino do potenciômetro
int lastPotValueE = 0; // Valor do potenciômetro da última leitura
const int threshold = 20; // Limiar de variação

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
  int b;
  int c;
  int d;
  int e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  int potValueA = analogRead(potPinA);
  int diffA = abs(potValueA - lastPotValueA);
  int potValueB = analogRead(potPinB);
  int diffB = abs(potValueB - lastPotValueB);
  int potValueC = analogRead(potPinC);
  int diffC = abs(potValueC - lastPotValueC);
  int potValueD = analogRead(potPinD);
  int diffD = abs(potValueD - lastPotValueD);
  int potValueE = analogRead(potPinE);
  int diffE = abs(potValueE - lastPotValueE);

  if (diffA >= threshold) {
    // Atualizar o valor anterior com o valor atual
    lastPotValueA = potValueA;
    // Printar o valor do potenciômetro apenas se houver uma mudança significativa
    Serial.print("Valor do potenciômetro A: ");
    Serial.println(potValueA);
  }

  if (diffB >= threshold) {
    // Atualizar o valor anterior com o valor atual
    lastPotValueB = potValueB;
    // Printar o valor do potenciômetro apenas se houver uma mudança significativa
    Serial.print("Valor do potenciômetro B: ");
    Serial.println(potValueB);
  }

  if (diffC >= threshold) {
    // Atualizar o valor anterior com o valor atual
    lastPotValueC = potValueC;
    // Printar o valor do potenciômetro apenas se houver uma mudança significativa
    Serial.print("Valor do potenciômetro C: ");
    Serial.println(potValueC);
  }

  if (diffD >= threshold) {
    // Atualizar o valor anterior com o valor atual
    lastPotValueD = potValueB;
    // Printar o valor do potenciômetro apenas se houver uma mudança significativa
    Serial.print("Valor do potenciômetro D: ");
    Serial.println(potValueD);
  }

  if (diffE >= threshold) {
    // Atualizar o valor anterior com o valor atual
    lastPotValueE = potValueE;
    // Printar o valor do potenciômetro apenas se houver uma mudança significativa
    Serial.print("Valor do potenciômetro E: ");
    Serial.println(potValueE);
  }
  // Set values to send
  myData.a = map(potValueA, 0, 4095, 0, 180);
  myData.b = map(potValueB, 0, 4095, 0, 180);
  myData.c = map(potValueC, 0, 4095, 0, 180);
  myData.d = map(potValueD, 0, 4095, 0, 180);
  myData.e = map(potValueE, 0, 4095, 0, 180);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}