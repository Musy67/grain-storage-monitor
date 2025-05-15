/* The system code aims to monitor the temperature, humidity and motion status on the storage facility. 
It then should display the values from the sensors in the dedicated display (LCD for now).
In case of any un-desired conditional change, the alert lights should come on and SMS alert should be sent to the system user.
*/

#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Definitions
#define DHT_PIN 2    
#define DHT_TYPE DHT11
#define PIR_PIN 3

// Object declarations
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function declarations
bool dhtSensorCheck(float temp, float humid);
void dhtMonitoring(float temp, float humid);
bool pirSensorCheck(int motion);
void pirMonitoring(int motion);

void setup() {
  // Components' initialization
  dht.begin();  
  lcd.init();
  pinMode(PIR_PIN, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
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

  // Read PIR value
  int motion = digitalRead(PIR_PIN);

  if (dhtSensorCheck(temp, humid)) {
    dhtMonitoring(temp, humid);
    delay(1500);
    lcd.clear();
  }

  if (pirSensorCheck(motion)) {
    pirMonitoring(motion);
    delay(1500);
    lcd.clear();
  }

  delay(750);
}

// Functions
bool dhtSensorCheck(float temp, float humid) { // Checks DHT sensor's integrity
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

bool pirSensorCheck(int motion) { // Checks integrity of PIR sensor
  if (isnan(motion)) {
    Serial.println("PIR sensor at fault!");
    lcd.setCursor(0, 0);
    lcd.print("No PIR detection");
    digitalWrite(4, HIGH);
    return false;
  }
  else {
    digitalWrite(4, LOW);
    return true;
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