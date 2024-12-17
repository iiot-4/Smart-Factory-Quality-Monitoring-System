#include <WiFi.h>
#include <PubSubClient.h>
#include "HX711.h"
#include <LiquidCrystal_PCF8574.h>

const char* ssid = "Factory2_2.4G";       
const char* password = "118factory2";   

const char* mqttServer = "192.168.50.149"; 
const int mqttPort = 1883;              
const char* mqttTopic = "sensor/weight";    

WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_PCF8574 lcd(0x27);

const int DT_PIN = 16;
const int SCK_PIN = 17;

const int scale_factor = 442; 

HX711 scale;

unsigned long lastPublishTime = 0;
unsigned long lastPromptTime = 0;

const float weightThreshold = 50.0;
float lastWeight = 0; 

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.clear();
  Serial.println("Initializing the scale");
  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.println(scale.get_units(5), 0);

  scale.set_scale(scale_factor); 
  scale.tare();                  

  Serial.println("After setting up the scale:");
  Serial.println(scale.get_units(5), 0);
  Serial.println("Readings:");

  float weight = scale.get_units(10);
  if (weight <= 0) weight = 0;
  lastWeight = weight;        

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.setCursor(9, 0);
  lcd.print(weight, 0);
  lcd.setCursor(13, 0);
  lcd.print("g");

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  unsigned long currentTime = millis();

  float weight = scale.get_units(10);
  if (weight <= 0) weight = 0; 

  if (currentTime - lastPromptTime >= 15000) { 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Change item");
    lastPromptTime = currentTime;
  }

  if (abs(weight - lastWeight) > weightThreshold) {
    delay(1000);
    float finalWeight = scale.get_units(10);
    if (finalWeight < 0) finalWeight = 0;
    if (finalWeight < 50.0) {
      delay(1000);
      finalWeight = scale.get_units(10);
      if (finalWeight < 0) finalWeight = 0;
    }

    lastWeight = finalWeight; 

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight: ");
    lcd.setCursor(9, 0);
    lcd.print(finalWeight, 0);
    lcd.setCursor(13, 0);
    lcd.print("g");
    char weightStr[8];
    dtostrf(finalWeight, 1, 2, weightStr);

    if (client.publish(mqttTopic, weightStr)) {
      Serial.println("MQTT æ¶æ¯åéæå: ");
      Serial.println(weightStr);
    } else {
      Serial.println("MQTT æ¶æ¯åéå¤±è´¥");
    }
  }

  scale.power_down();
  delay(500);
  scale.power_up();
}

void connectToWiFi() {
  Serial.print("æ­£å¨è¿æ¥å° Wi-Fi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi è¿æ¥æå");
  Serial.print("åéç IP å°å: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.print("æ­£å¨è¿æ¥å° MQTT æå¡å¨: ");
  Serial.println(mqttServer);

  while (!client.connected()) {
    if (client.connect("ESPduinoClient")) {  
      Serial.println("MQTT è¿æ¥æå");
      client.subscribe("sensor/weight");
    } else {
      Serial.print("MQTT è¿æ¥å¤±è´¥ï¼ç¶æç  = ");
      Serial.println(client.state());
      delay(5000); 
    }
  }
}
