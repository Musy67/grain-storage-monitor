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
bool dhtSensorCheck(float temp, float humid);
void dhtMonitoring(float temp, float humid);

void setup() {
  // Components' initialization
  dht.begin();  
  lcd.init();
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
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

  if (dhtSensorCheck(temp, humid)) {
    dhtMonitoring(temp, humid);
    delay(1500);
    lcd.clear();
  }

  delay(750);
}

// Functions
bool dhtSensorCheck(float temp, float humid) { // Checks dht sensor's integrity
  if (isnan(temp) || isnan(humid)) {
    Serial.println("DHT sensor at fault!");
    lcd.setCursor(0, 0);
    lcd.print("No DHT readings!");
    digitalWrite(4, HIGH);
    return false;
  }
  else {
    digitalWrite(4, LOW);
    return true;
  }
}

void dhtMonitoring(float temp, float humid) { // Monitors and displays temperature and humidity
  lcd.setCursor(0, 0);
  String temperature = "Temp: " + String(temp) + "°C";
  lcd.print(temperature);

  if ((temp > 30) || (temp < 10)) {
    digitalWrite(5, HIGH);
    Serial.println("Temperature out of range, " + temperature);
  }
  else {
    digitalWrite(6, LOW);
  }

  lcd.setCursor(0, 1);
  String humidity = "Humid: " + String(humid) + "%";
  lcd.print(humidity);

  if ((humid > 60) || (humid < 40)){
    digitalWrite(6, HIGH);
    Serial.println("Humidity out of range, " + humidity);
  }
  else {
    digitalWrite(6, LOW);
  }
}