#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>  // Zmienione z WiFiUDP.h na WiFiUdp.h

// Konfiguracja Access Pointa
const char *ssid = "Robot_ESP32_S3";
const char *password = "12345678";

WiFiUDP udp; // Klasa zostaje taka sama
const unsigned int localUdpPort = 4210;
char incomingPacket[255];

// Piny dla L293D (S3)
const int AIN1 = 4;
const int AIN2 = 5;
const int BIN1 = 6;
const int BIN2 = 7;

unsigned long lastPacketTime = 0;
const unsigned long timeoutMs = 500;

void setMotor(int speed, int pin1, int pin2) {
    speed = constrain(speed, -255, 255);
    if (speed > 0) {
        analogWrite(pin1, speed);
        analogWrite(pin2, 0);
    } else if (speed < 0) {
        analogWrite(pin1, 0);
        analogWrite(pin2, abs(speed));
    } else {
        analogWrite(pin1, 0);
        analogWrite(pin2, 0);
    }
}

void setup() {
    Serial.begin(115200);
    
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    Serial.println("\nUruchamianie AP...");
    if(WiFi.softAP(ssid, password)) {
        Serial.print("IP: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("Błąd AP!");
    }

    udp.begin(localUdpPort);
}

void loop() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = 0;
            lastPacketTime = millis();

            int sA = 0, sB = 0;
            if (sscanf(incomingPacket, "%d,%d", &sA, &sB) == 2) {
                setMotor(sA, AIN1, AIN2);
                setMotor(sB, BIN1, BIN2);
            }
        }
    }

    if (millis() - lastPacketTime > timeoutMs) {
        setMotor(0, AIN1, AIN2);
        setMotor(0, BIN1, BIN2);
    }

    delay(20);
}

/*#include <Arduino.h>
#include <Bluepad32.h>

const int AIN1 = 4;
const int AIN2 = 5;

const int BIN1 = 6;
const int BIN2 = 7;

const int freq = 5000;
const int ledc_resolution = 8;

ControllerPtr myController;

void onConnectedController(ControllerPtr ctl) {
  if (myController == nullptr) {
    myController = ctl;
    Serial.println("Polaczono pada!");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  if (myController == ctl) {
    myController = nullptr;
    Serial.println("Rozlaczono pada!");
  }
}

void setMotor(int speed, int pin1, int pin2) {
  if(pin1 == AIN1) {
    Serial.printf("Engine 1: %d\t", speed);
  }
  else {
    Serial.printf("Engine 2: %d\n", speed);
  }

  if (speed > 0) {
    // Jazda do przodu
    analogWrite(pin1, speed);
    analogWrite(pin2, 0);
  } else if (speed < 0) {
    // Jazda do tyłu
    analogWrite(pin1, 0);
    analogWrite(pin2, abs(speed));
  } else {
    // Stop
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial); 
  delay(2000);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  setMotor(0, AIN1, AIN2);
  setMotor(0, BIN1, BIN2);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  //BP32.forgetBluetoothKeys();
  
  Serial.println("Gotowy! Na wyłączonym padzie wcisnij i trzymaj SHARE + DOMEK az zacznie szybko migac...");
}

void loop() {
  BP32.update();

  bool hasController = false;
  if (myController != nullptr && myController->isConnected()) {
    hasController = true;
    
    if(myController->a()) {
      setMotor(0, AIN1, AIN2);
      setMotor(0, BIN1, BIN2);
    }

    else {
      int leftY = myController->axisY();
      int rightY = myController->axisRY();
      if (abs(leftY) < 40)
        leftY = 0;
      if (abs(rightY) < 40)
        rightY = 0;
      int speedA = map(leftY, 512, -511, -255, 255);
      int speedB = map(rightY, 512, -511, -255, 255);
      //Serial.println(speedA);
      //Serial.println(speedB);
      setMotor(speedA, AIN1, AIN2);
      setMotor(speedB, BIN1, BIN2);
    }
  }

  if (!hasController) {
    setMotor(0, AIN1, AIN2);
    setMotor(0, BIN1, BIN2);
  }
  
  delay(20);
}*/


//TEST BLUETOOTH:
// #include <Arduino.h>
// #include "BluetoothSerial.h"

// BluetoothSerial SerialBT;

// void setup() {
//   Serial.begin(115200);
//   while(!Serial);
//   delay(1000);

//   // Odpalamy Bluetooth pod wybraną nazwą
//   SerialBT.begin("ESP32_TEST_BT"); 
  
//   Serial.println("\n--- TEST BLUETOOTH ---");
//   Serial.println("Modul Bluetooth uruchomiony!");
//   Serial.println("Wejdz w ustawienia komputera i poszukaj urządzenia: ESP32_TEST_BT");
// }

// void loop() {
//   // Pusta pętla - ESP32 po prostu nadaje sygnał
//   delay(1000);
// }


// #include <Arduino.h>
// #include <PS4Controller.h>

// const int PWMA = 25;
// const int AIN1 = 26;
// const int AIN2 = 27;

// const int PWMB = 14;
// const int BIN1 = 12;
// const int BIN2 = 13;

// const int freq = 5000;
// const int pwmChannelA = 0;
// const int pwmChannelB = 1;
// const int ledc_resolution = 8;

// void setMotorA(int speed) {
//   if (speed > 0) {
//     digitalWrite(AIN1, HIGH);
//     digitalWrite(AIN2, LOW);
//     ledcWrite(pwmChannelA, speed);
//   } else if (speed < 0) {
//     digitalWrite(AIN1, LOW);
//     digitalWrite(AIN2, HIGH);
//     ledcWrite(pwmChannelA, abs(speed));
//   } else {
//     digitalWrite(AIN1, LOW);
//     digitalWrite(AIN2, LOW);
//     ledcWrite(pwmChannelA, 0);
//   }
// }

// void setMotorB(int speed) {
//   if (speed > 0) {
//     digitalWrite(BIN1, HIGH);
//     digitalWrite(BIN2, LOW);
//     ledcWrite(pwmChannelB, speed);
//   } else if (speed < 0) {
//     digitalWrite(BIN1, LOW);
//     digitalWrite(BIN2, HIGH);
//     ledcWrite(pwmChannelB, abs(speed));
//   } else {
//     digitalWrite(BIN1, LOW);
//     digitalWrite(BIN2, LOW);
//     ledcWrite(pwmChannelB, 0);
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   while(!Serial); 
//   delay(2000);

//   pinMode(AIN1, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);

//   ledcSetup(pwmChannelA, freq, ledc_resolution);
//   ledcAttachPin(PWMA, pwmChannelA);

//   ledcSetup(pwmChannelB, freq, ledc_resolution);
//   ledcAttachPin(PWMB, pwmChannelB);

//   setMotorA(0);
//   setMotorB(0);

//   PS4.begin("3C:8A:1F:09:7E:36"); 
  
//   Serial.println("Gotowy! Wcisnij przycisk PS na padzie, aby polaczyc...");
// }

// void loop() {
//   if (PS4.isConnected()) {
//     Serial.println("polaczone!");
//     Serial.println();

//     int leftY = PS4.LStickY();  
//     int rightY = PS4.RStickY(); 

//     if (abs(leftY) < 15) leftY = 0;
//     if (abs(rightY) < 15) rightY = 0;

//     int speedA = map(leftY, -128, 127, -255, 255);
//     int speedB = map(rightY, -128, 127, -255, 255);

//     setMotorA(speedA);
//     setMotorB(speedB);
//   } else {
//     setMotorA(0);
//     setMotorB(0);
//   }
  
//   delay(20);
// }


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