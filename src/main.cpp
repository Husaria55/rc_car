#include <Arduino.h>
#include <esp_mac.h>

// bluetooth debug
uint8_t mac[6];

void setup() {
  Serial.begin(115200);
  while(!Serial); 
  delay(2000);
  Serial.println("\n\n--- TEST SERIAL OK ---");

  // bluetooth debug
  esp_read_mac(mac, ESP_MAC_BT);  
}

void loop() {
  // bluetooth debug
  Serial.printf("TWOJ BT MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  delay(1000);
}


// // #include <PS4Controller.h>
// #include <Arduino.h>


// // Piny dla Silnika A (Lewy)
// int PWMA = 25;
// int AIN1 = 26;
// int AIN2 = 27;

// // Piny dla Silnika B (Prawy)
// int PWMB = 14;
// int BIN1 = 12;
// int BIN2 = 13;

// // Ustawienia PWM
// const int freq = 5000;
// const int ledc_resolution = 8; // 0-255


// void setup() {
//   Serial.begin(115200);
//   delay(1000); 

//   uint8_t mac[6];
//   esp_read_mac(mac, ESP_MAC_BT); 
  
//   Serial.print("ADRES MAC TWOJEGO ESP32 (Bluetooth): ");
//   for (int i = 0; i < 6; i++) {
//     Serial.printf("%02X", mac[i]);
//     if (i < 5) Serial.print(":");
//   }
//   Serial.println();

//   // piny
//   pinMode(AIN1, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);
//   ledcSetup(0, freq, ledc_resolution);
//   ledcSetup(1, freq, ledc_resolution);
//   ledcAttachPin(PWMA, 0);
//   ledcAttachPin(PWMB, 1);

//   // Serial.print("Adres MAC WiFi: ");
//   // Serial.println(WiFi.macAddress());

//   // Podaj adres MAC swojego pada lub zostaw puste, jeśli sparowałeś ESP z padem
//   // PS4.begin("00:1f:e2:f2:61:c0");

//   Serial.println("Gotowy! Polacz pada PS4...");
// }


// void moveMotors(int leftSpeed, int rightSpeed) {
//   // Silnik Lewy
//   if (leftSpeed >= 0) {
//     digitalWrite(AIN1, HIGH);
//     digitalWrite(AIN2, LOW);
//     ledcWrite(0, leftSpeed);
//   } else {
//     digitalWrite(AIN1, LOW);
//     digitalWrite(AIN2, HIGH);
//     ledcWrite(0, abs(leftSpeed));
//   }

//   // Silnik Prawy
//   if (rightSpeed >= 0) {
//     digitalWrite(BIN1, HIGH);
//     digitalWrite(BIN2, LOW);
//     ledcWrite(1, rightSpeed);
//   } else {
//     digitalWrite(BIN1, LOW);
//     digitalWrite(BIN2, HIGH);
//     ledcWrite(1, abs(rightSpeed));
//   }
// }

// void loop() {
//   if (PS4.isConnected()) {
//     // Odczyt gałek (zakres -127 do 127)
//     int yAxis = PS4.LStickY();
//     int xAxis = PS4.LStickX();

//     // Prosty mikser dla sterowania czołgowego
//     int leftMotorSpeed = yAxis + xAxis;
//     int rightMotorSpeed = yAxis - xAxis;

//     // Mapowanie na zakres PWM (0-255) i ograniczenie
//     leftMotorSpeed = map(leftMotorSpeed, -127, 127, -255, 255);
//     rightMotorSpeed = map(rightMotorSpeed, -127, 127, -255, 255);

//     // Martwa strefa (żeby auto nie drgało samo)
//     if (abs(yAxis) < 15 && abs(xAxis) < 15) {
//       moveMotors(0, 0);
//     } else {
//       moveMotors(leftMotorSpeed, rightMotorSpeed);
//     }
//   }
//   delay(20);
// }


//TESTOWY:
// #include <Arduino.h>

// // Piny dla Silnika A (Lewy)
// int PWMA = 25;
// int AIN1 = 26;
// int AIN2 = 27;

// // Piny dla Silnika B (Prawy)
// int PWMB = 14;
// int BIN1 = 12;
// int BIN2 = 13;

// // Ustawienia PWM
// const int freq = 5000;
// const int ledc_channel_A = 0;
// const int ledc_channel_B = 1;
// const int ledc_resolution = 8; 

// void moveMotors(int leftSpeed, int rightSpeed) {
//   // Silnik Lewy (A)
//   if (leftSpeed >= 0) {
//     digitalWrite(AIN1, HIGH);
//     digitalWrite(AIN2, LOW);
//     ledcWrite(ledc_channel_A, leftSpeed);
//   } else {
//     digitalWrite(AIN1, LOW);
//     digitalWrite(AIN2, HIGH);
//     ledcWrite(ledc_channel_A, abs(leftSpeed));
//   }

//   // Silnik Prawy (B)
//   if (rightSpeed >= 0) {
//     digitalWrite(BIN1, HIGH);
//     digitalWrite(BIN2, LOW);
//     ledcWrite(ledc_channel_B, rightSpeed);
//   } else {
//     digitalWrite(BIN1, LOW);
//     digitalWrite(BIN2, HIGH);
//     ledcWrite(ledc_channel_B, abs(rightSpeed));
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   Serial.println("--- TEST SILNIKOW START ---");

//   pinMode(AIN1, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);

//   // Konfiguracja PWM
//   ledcSetup(ledc_channel_A, freq, ledc_resolution);
//   ledcSetup(ledc_channel_B, freq, ledc_resolution);
//   ledcAttachPin(PWMA, ledc_channel_A);
//   ledcAttachPin(PWMB, ledc_channel_B);
// }

// void loop() {
//   Serial.println("Do przodu (50% mocy)...");
//   moveMotors(150, 150); 
//   delay(2000);

//   Serial.println("Stop...");
//   moveMotors(0, 0);
//   delay(1000);

//   Serial.println("Do tyłu (50% mocy)...");
//   moveMotors(-150, -150);
//   delay(2000);

//   Serial.println("Stop...");
//   moveMotors(0, 0);
//   delay(3000);
// }