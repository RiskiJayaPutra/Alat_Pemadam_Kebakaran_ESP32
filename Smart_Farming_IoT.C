#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "GANTI_DENGAN_ID_TEMPLATE"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "GANTI_DENGAN_TOKEN_KAMU"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Pin dan konfigurasi
const int soilPin = A0;
const int threshold = 500;
const int servoPin = D7; // GPIO13

// Objek dan variabel
Servo keranServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// WiFi credentials
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

void sendSoilData() {
  int soilMoisture = analogRead(soilPin);
  
  // Tampilkan di Serial dan LCD
  Serial.print("Kelembaban: ");
  Serial.println(soilMoisture);
  
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(soilMoisture);
  lcd.print("   ");
  
  lcd.setCursor(0, 1);

  if (soilMoisture < threshold) {
    keranServo.write(90);
    lcd.print("Status: Kering ");
    Serial.println("Tanah kering - Keran dibuka");

    Blynk.virtualWrite(V0, "Tanah Kering - Keran Dibuka");
    Blynk.virtualWrite(V1, soilMoisture);
  } else {
    keranServo.write(0);
    lcd.print("Status: Lembab ");
    Serial.println("Tanah lembab - Keran ditutup");

    Blynk.virtualWrite(V0, "Tanah Lembab - Keran Tertutup");
    Blynk.virtualWrite(V1, soilMoisture);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  delay(2000);
  lcd.clear();

  keranServo.attach(servoPin);
  keranServo.write(0); // posisi awal tertutup

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSoilData); // setiap 2 detik kirim data
}

void loop() {
  Blynk.run();
  timer.run();
}
