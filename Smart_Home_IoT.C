#define BLYNK_TEMPLATE_ID "TMPL6M3UT5qS4"
#define BLYNK_TEMPLATE_NAME "UAP"
#define BLYNK_AUTH_TOKEN "7vY1KnQ5mmm9dIqjEZZUsIgGjQqWIE_2"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pin dan Konstanta
const int pirPin = D2;      // GPIO4 (digital input PIR sensor)
const int lampPin = D3;     // GPIO0 (lampu LED)
const int buzzerPin = D4;   // GPIO2 (buzzer)
const int servoPin = D5;    // GPIO14 (servo)

// Objek dan Variabel
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servoMotor;
BlynkTimer timer;

bool motionPreviouslyDetected = false;

void sendStatus() {
  int motionDetected = digitalRead(pirPin);
  Blynk.virtualWrite(V1, motionDetected);        // Status PIR sensor (1/0)
  Blynk.virtualWrite(V2, digitalRead(lampPin));  // Status lampu (1/0)
  Blynk.virtualWrite(V3, motionPreviouslyDetected ? 90 : 0); // Posisi servo
}

void setup() {
  Serial.begin(9600);
  
  pinMode(pirPin, INPUT);
  pinMode(lampPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  servoMotor.attach(servoPin);
  servoMotor.write(0);  // Pintu tertutup

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Door Ready");
  delay(2000);
  lcd.clear();

  Blynk.begin(BLYNK_AUTH_TOKEN, "Wokwi-GUEST", ""); // Ganti ssid dan password sesuai

  timer.setInterval(1000L, sendStatus);
}

void loop() {
  Blynk.run();
  timer.run();

  int motionDetected = digitalRead(pirPin);

  lcd.setCursor(0, 0);
  lcd.print("Status:            ");
  lcd.setCursor(0, 1);

  if (motionDetected == HIGH) {
    digitalWrite(lampPin, HIGH);
    servoMotor.write(90);  // Pintu terbuka
    lcd.print("Selamat Datang!   ");
    Serial.println("Gerakan Terdeteksi: Pintu Dibuka");

    if (!motionPreviouslyDetected) {
      tone(buzzerPin, 1000);  // Bunyi 1 detik
      delay(1000);
      noTone(buzzerPin);
      motionPreviouslyDetected = true;
    }
  } else {
    digitalWrite(lampPin, LOW);
    servoMotor.write(0);  // Pintu tertutup
    lcd.print("Tidak Ada Orang   ");
    Serial.println("Tidak Ada Gerakan: Pintu Ditutup");
    motionPreviouslyDetected = false;
  }

  delay(200);
}
