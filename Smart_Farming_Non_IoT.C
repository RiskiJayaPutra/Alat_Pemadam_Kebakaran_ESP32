#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

const int soilPin = A0;        // Pin sensor kelembaban tanah
const int threshold = 500;     // Nilai ambang batas tanah kering
Servo keranServo;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  delay(2000);
  lcd.clear();

  keranServo.attach(7);
  keranServo.write(0);
}

void loop() {
  int soilMoisture = analogRead(soilPin);
  
  Serial.print("Kelembaban: ");
  Serial.println(soilMoisture);

  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(soilMoisture);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  if (soilMoisture < threshold) {
    keranServo.write(90);
    lcd.print("Status: Kering   ");
    Serial.println("Tanah kering - Keran dibuka");
  } else {
    keranServo.write(0);
    lcd.print("Status: Lembab   ");
    Serial.println("Tanah lembab - Keran ditutup");
  }
  delay(1000);
}
