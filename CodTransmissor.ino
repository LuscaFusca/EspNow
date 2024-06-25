#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x08, 0xd1, 0xf9, 0x27, 0xc9, 0xd4};

#define pinSensorCima 22 // Pino da bomba de cima
#define pinSensorBaixo 23 // Pino da bomba de baixo


bool sensorCima = false;
bool sensorBaixo = false;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool a;
  bool b;
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
  pinMode(pinSensorCima, INPUT_PULLUP);
  pinMode(pinSensorBaixo, INPUT_PULLUP);

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
  /*
  bool bombaCima = digitalRead(pinBombaCima);
  bool bombaBaixo = digitalRead(pinBombaCima);
  */
  char valor = Serial.read();
  if(valor == 'q'){
    sensorCima = true;
  }else if(valor == 'w'){
    sensorCima = false;
  }else if(valor == 'a'){
    sensorBaixo = true;
  }else if(valor == 's'){
    sensorBaixo = false;
  }
   
  

  Serial.print("Valor da bomba de cima ");
  Serial.println(sensorCima);
  Serial.print("Valor da bomba de baixo ");
  Serial.println(sensorBaixo);
  

  // Set values to send
  myData.a = sensorCima;
  myData.b = sensorBaixo;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}