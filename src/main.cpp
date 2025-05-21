#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Definitions
#define DHT_PIN 2    
#define DHT_TYPE DHT11
#define PIR_PIN 3 
#define GAS_PIN A6

// Object declarations
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

// Function declarations
bool dhtSensorCheck(float temp, float humid);
void dhtMonitoring(float temp, float humid);
void pirMonitoring(int motion);
void gasMonitoring(int gasValue);

void setup() {
  // Components' initialization
  dht.begin();
  lcd.begin(16, 2); 
  lcd.backlight();
  pinMode(PIR_PIN, INPUT);
  pinMode(4, OUTPUT);       // Sensor faults
  pinMode(5, OUTPUT);       // Temp out of range    
  pinMode(6, OUTPUT);       // Humidity out of range
  pinMode(7, OUTPUT);       // Motion detected
  pinMode(8, OUTPUT);       // Spoilage detection
  Serial.begin(9600);
  
  // Displays welcome message
  lcd.setCursor(1, 0);
  lcd.print("Grain Storage");
  lcd.setCursor(5, 1);
  lcd.print("Monitor!");
  delay(500);
}

void loop() {
  lcd.clear();

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (dhtSensorCheck(temp, humid)) {
    dhtMonitoring(temp, humid); // Display DHT values and report unusual occurence
    delay(500);
    lcd.clear(); 
  }

  // Read PIR value
  int motion = digitalRead(PIR_PIN);
  pirMonitoring(motion);      // Display PIR values and report unusual occurence

  // Read gas value
  int gasValue = analogRead(GAS_PIN);
  gasMonitoring(gasValue);    // Display GAS values and report unusual occurence

  delay(750);
}

// Functions
bool dhtSensorCheck(float temp, float humid) { // Checks sensors' integrity
  lcd.setCursor(0, 0);
  if (isnan(temp) || isnan(humid)) {
    Serial.println("DHT sensor at fault!");
    lcd.print("No DHT readings!");
    delay(500);
    lcd.clear();
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
    digitalWrite(5, LOW);
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

void pirMonitoring(int motion) { // Checks and displays motion status
  lcd.setCursor(0, 0);
  if (motion) {
    Serial.println("Motion detected!");
    lcd.print("Rodent present!");
    digitalWrite(7, HIGH);
  }
  else {
    lcd.print("No motion!");
    digitalWrite(7, LOW);
  }
}

void gasMonitoring(int gasValue) {
  lcd.setCursor(0, 1);
  if (gasValue > 500) {
    Serial.println("Spoilage detected!");
    lcd.print("Grain spoiled!");
    digitalWrite(8, HIGH); 
  }
  else {
    lcd.print("No spoilage!");
    digitalWrite(8, LOW);
  }
}