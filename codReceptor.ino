#include <esp_now.h>
#include <WiFi.h>

#define pinBomba 23

// Estrutura exemplo para receber dados
// Deve corresponder à estrutura do remetente
typedef struct struct_message {
    bool a;
    bool b;
} struct_message;

// Crie uma struct_message chamada myData
struct_message myData;

// Função de callback que será executada quando os dados forem recebidos
void OnDataRecv(const esp_now_recv_info * info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  
    Serial.print("Sensor Cima: ");
    Serial.println(myData.a);
  
  
  
    Serial.print("Sensor Baixo: ");
    Serial.println(myData.b);
  
}
 
void setup() {
  // Inicializa o Monitor Serial
  Serial.begin(115200);
  pinMode(pinBomba, OUTPUT);
  // Configura o dispositivo como uma estação Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }
  
  // Uma vez que o ESPNow é inicializado com sucesso, registramos para receber CB para
  // obter informações do pacote recebido
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
