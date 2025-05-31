#define BLYNK_TEMPLATE_ID "TMPL6M3UT5qS4"
#define BLYNK_TEMPLATE_NAME "UAP"
#define BLYNK_AUTH_TOKEN "7vY1KnQ5mmm9dIqjEZZUsIgGjQqWIE_2"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>                         // GANTI dari DHTesp.h
#include <LiquidCrystal_I2C.h>

// Pin dan Konstanta
#define DHTPIN D4                        // GPIO2
#define DHTTYPE DHT22                    // Jenis sensor
#define BUZZER D5                        // GPIO14
#define LED_MERAH D6                     // GPIO12

// Objek dan Variabel
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);               // GANTI: pakai objek DHT biasa
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

void sendSensor() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  Serial.println("Temp: " + String(temperature, 2) + "°C");
  Serial.println("Humidity: " + String(humidity, 1) + "%");
  Serial.println("---");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(temperature, 2) + " C");

  lcd.setCursor(0, 1);
  if (temperature < 20) {
    lcd.print("AMAN");
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V0, temperature);
  }
  else if (temperature > 32) {
    lcd.print("BAHAYA API!!!");
    digitalWrite(LED_MERAH, HIGH);
    tone(BUZZER, 165);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, temperature);
  }
  else {
    lcd.print("TIDAK ADA API");
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V0, temperature);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  dht.begin();                          // INISIALISASI DHT
  lcd.init();
  lcd.backlight();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
