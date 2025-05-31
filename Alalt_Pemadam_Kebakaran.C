
#define BLYNK_TEMPLATE_ID "TMPL6M3UT5qS4"
#define BLYNK_TEMPLATE_NAME "UAP"
#define BLYNK_AUTH_TOKEN "7vY1KnQ5mmm9dIqjEZZUsIgGjQqWIE_2"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>

const int DHT_PIN = D4;         // pin GPIO2
const int BUZZER = D5;          // pin GPIO14
const int LED_MERAH = D6;       // pin GPIO12

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

void sendSensor() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(data.temperature, 2) + " C");

  lcd.setCursor(0, 1);
  if (data.temperature < 20) {
    lcd.print("AMAN");
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V0, data.temperature);
  }
  else if (data.temperature > 32) {
    lcd.print("BAHAYA API!!!");
    digitalWrite(LED_MERAH, HIGH);
    tone(BUZZER, 165);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, data.temperature);
  }
  else {
    lcd.print("TIDAK ADA API");
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V0, data.temperature);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  lcd.init();
  lcd.backlight();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
