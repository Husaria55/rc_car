#include <Arduino.h>
#include <PS4Controller.h>

// --- KONFIGURACJA PINÓW ---

// Piny dla Silnika A (Lewy)
const int PWMA = 25;
const int AIN1 = 26;
const int AIN2 = 27;

// Piny dla Silnika B (Prawy)
const int PWMB = 14;
const int BIN1 = 12;
const int BIN2 = 13;

// Ustawienia PWM
const int freq = 5000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int ledc_resolution = 8; // Wartości od 0 do 255

// --- FUNKCJE STERUJĄCE SILNIKAMI ---

// Funkcja sterująca lewym silnikiem (prędkość od -255 do 255)
void setMotorA(int speed) {
  if (speed > 0) { // Do przodu
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(pwmChannelA, speed);
  } else if (speed < 0) { // Do tyłu
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(pwmChannelA, abs(speed));
  } else { // Stop
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    ledcWrite(pwmChannelA, 0);
  }
}

// Funkcja sterująca prawym silnikiem (prędkość od -255 do 255)
void setMotorB(int speed) {
  if (speed > 0) { // Do przodu
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(pwmChannelB, speed);
  } else if (speed < 0) { // Do tyłu
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(pwmChannelB, abs(speed));
  } else { // Stop
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(pwmChannelB, 0);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial); 
  delay(2000);

  // --- KONFIGURACJA PINÓW I PWM ---
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Konfiguracja kanałów PWM dla silników
  ledcSetup(pwmChannelA, freq, ledc_resolution);
  ledcAttachPin(PWMA, pwmChannelA);

  ledcSetup(pwmChannelB, freq, ledc_resolution);
  ledcAttachPin(PWMB, pwmChannelB);

  // Zatrzymanie silników na starcie
  setMotorA(0);
  setMotorB(0);

  // --- START BLUETOOTH ---
  // TUTAJ WPISZ ADRES MAC ESP32 (TEN SAM, KTÓRY WPISAŁEŚ DO PADA!)
  PS4.begin("3C:8A:1F:09:7E:36"); // Zamień na swój MAC!
  
  Serial.println("Gotowy! Wcisnij przycisk PS na padzie, aby polaczyc...");
}

void loop() {
  // Sprawdzamy, czy pad jest połączony
  if (PS4.isConnected()) {
    
    // Odczyt gałek - pad PS4 zwraca wartości od -128 (dół) do 127 (góra)
    int leftY = PS4.LStickY();  
    int rightY = PS4.RStickY(); 

    // Deadzone (martwa strefa) - stare pady potrafią "pływać" i nie wracają idealnie do zera.
    // Dzięki temu silniki nie będą cicho buczeć, gdy puścicie gałki.
    if (abs(leftY) < 15) leftY = 0;
    if (abs(rightY) < 15) rightY = 0;

    // Mapujemy zakres odczytu z pada (-128 do 127) na zakres mocy PWM (-255 do 255)
    int speedA = map(leftY, -128, 127, -255, 255);
    int speedB = map(rightY, -128, 127, -255, 255);

    // Wysyłamy moc na silniki
    setMotorA(speedA);
    setMotorB(speedB);
    
    // Opcjonalny podgląd w Serial Monitorze
    // Serial.printf("Moc L: %d | Moc R: %d\n", speedA, speedB);
  } else {
    // Jeśli pad zgubi zasięg lub się rozłączy – dla bezpieczeństwa zatrzymujemy silniki!
    setMotorA(0);
    setMotorB(0);
  }
  
  // Mały delay, żeby nie zasypać procesora
  delay(20);
}


// #include <Arduino.h>
// #include <esp_mac.h>

// // bluetooth debug
// uint8_t mac[6];

// void setup() {
//   Serial.begin(115200);
//   while(!Serial); 
//   delay(2000);
//   Serial.println("\n\n--- TEST SERIAL OK ---");

//   // bluetooth debug
//   esp_read_mac(mac, ESP_MAC_BT);  
// }

// void loop() {
//   // bluetooth debug
//   Serial.printf("TWOJ BT MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", 
//                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//   delay(1000);
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