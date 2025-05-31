#include <Servo.h>
#include <LiquidCrystal_I2C.h>

const int pirPin = 2;         // Pin sensor PIR
const int lampPin = 8;        // Pin lampu (LED)
const int buzzerPin = 9;      // Pin buzzer
const int servoPin = 10;      // Pin servo

#define I2C_ADDR    0x27 
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
Servo servoMotor;

bool motionPreviouslyDetected = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(pirPin, INPUT);
  pinMode(lampPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  servoMotor.attach(servoPin);
  servoMotor.write(0);  // Pintu tertutup

  lcd.setCursor(0, 0);
  lcd.print("Smart Door Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
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
      tone(buzzerPin, 1000);     // Bunyi 1 detik
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

  delay(200);  // Delay kecil agar tidak terlalu sering polling
}
