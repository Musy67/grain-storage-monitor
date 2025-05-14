#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Definitions
#define DHT_PIN 2     
#define DHT_TYPE DHT11

// Object declarations
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function declarations
void dhtSensorCheck(float temp, float humid);

void setup() {
  // Components' initialization
  dht.begin();  
  lcd.init();
  Serial.begin(9600);
  
  lcd.setCursor(0, 0);
  lcd.print("Grain Storage");
  lcd.setCursor(5, 1);
  lcd.print("Monitor!");
  delay(1000);
}

void loop() {
  lcd.clear();

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  
  dhtSensorCheck(temp, humid); // Checks dht sensor's integrity
}

void dhtSensorCheck(float temp, float humid) {
  if (isnan(temp) || isnan(humid)) {
    Serial.println("DHT sensor at fault!");
    lcd.setCursor(0, 0);
    lcd.print("No DHT readings!");
  }
}