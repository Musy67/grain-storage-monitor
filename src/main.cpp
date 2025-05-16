#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Definitions
#define DHT_PIN 2    
#define DHT_TYPE DHT11
#define PIR_PIN 3
#define sim900_RX 13
#define sim900_TX 12  

// Object declarations
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial simModule(sim900_RX, sim900_TX); // (TX, RX)

// Global variables
const String phoneNum = "+255773422381";

// Function declarations
bool sensorCheck(float temp, float humid, int motion);
void dhtMonitoring(float temp, float humid);
void pirMonitoring(int motion);
void sendSMS(String phoneNum, String message);

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
  simModule.begin(9600);
  
  // Displays welcome message
  lcd.setCursor(0, 0);
  lcd.print("Grain Storage");
  lcd.setCursor(5, 1);
  lcd.print("Monitor!");
  delay(1000);

  // Test for SIM900 module
  sendSMS(phoneNum, "Test for SMS!");
}

void loop() {
  lcd.clear();

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  // Read PIR value
  int motion = digitalRead(PIR_PIN);

  if (sensorCheck(temp, humid, motion)) {
    dhtMonitoring(temp, humid); // Display DHT values and report unusual occurence
    delay(1500);
    lcd.clear(); 

    pirMonitoring(motion);      // Display PIR values and report unusual occurence
    delay(1500);
    lcd.clear();
  }

  delay(750);
}

// Functions
bool sensorCheck(float temp, float humid, int motion) { // Checks sensors' integrity
  lcd.setCursor(0, 0);
  if (isnan(temp) || isnan(humid)) {
    Serial.println("DHT sensor at fault!");
    lcd.print("No DHT readings!");
    digitalWrite(4, HIGH);
    return false;
  }
  else if (isnan(motion)) {
    Serial.println("PIR sensor at fault");
    lcd.print("No PIR detection");
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

void sendSMS(String phoneNum, String message) {
  simModule.println("AT");  // Check comms
  delay(500);

  simModule.println("AT+CMGF=1"); // Set text mode
  delay(500);

  simModule.println("AT+CMGS=1\"" + phoneNum + "\""); // Set phone number
  delay(500);

  simModule.print(message);
  delay(500);
  simModule.write(26);  // Ctrl+Z to send
  delay(5000);  
}