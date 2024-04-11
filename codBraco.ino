#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#define pinServoBase 23
#define pinServoOmbro 22
#define pinServoAntBraco 18
#define pinServoBraco 21
#define pinServoGarra 19

Servo servoBase;
Servo servoOmbro;
Servo servoAntBraco;
Servo servoBraco;
Servo servoGarra;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
    int b;
    int c;
    int d;
    int e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("A: ");
  Serial.println(myData.a);
  servoBase.write(myData.a);
  Serial.print("B: ");
  Serial.println(myData.b);
  servoOmbro.write(myData.b);
  Serial.print("C: ");
  Serial.println(myData.c);
  servoAntBraco.write(myData.c);
  Serial.print("D: ");
  Serial.println(myData.d);
  servoBraco.write(myData.d);
  Serial.print("E: ");
  Serial.println(myData.e);
  servoGarra.write(myData.e);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  servoBase.attach(pinServoBase);
  servoOmbro.attach(pinServoOmbro);
  servoAntBraco.attach(pinServoAntBraco);
  servoBraco.attach(pinServoBraco);
  servoGarra.attach(pinServoGarra);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}