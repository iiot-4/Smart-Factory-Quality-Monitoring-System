#include <WiFi.h>
#include <PubSubClient.h>
#include "HX711.h"
#include <LiquidCrystal_PCF8574.h>

const char* ssid = "Your Wi-Fi SSID ";      
const char* password = "Your Wi-Fi Password";   


const char* mqttServer = "Your MQTT IP"; 
const int mqttPort = 1883;               
const char* mqttTopic = "sensor/weight";   

WiFiClient espClient;
PubSubClient client(espClient);


LiquidCrystal_PCF8574 lcd(0x27);  


const int DT_PIN = 16;
const int SCK_PIN = 17;

const int scale_factor = 443; 

HX711 scale;


unsigned long lastPublishTime = 0;
unsigned long lastPromptTime = 0;


const float weightThreshold = 10.0; // ±10g
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
    lastWeight = weight; 

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight: ");
    lcd.setCursor(9, 0);
    lcd.print(weight, 0);
    lcd.setCursor(13, 0);
    lcd.print("g");

    
    char weightStr[8];
    dtostrf(weight, 1, 2, weightStr);

    
    if (client.publish(mqttTopic, weightStr)) {
      Serial.println("MQTT 訊息發送成功: ");
      Serial.println(weightStr);
    } else {
      Serial.println("MQTT 訊息發送失敗");
    }
  }

  
  scale.power_down();
  delay(500);
  scale.power_up();
}

void connectToWiFi() {
  Serial.print("正在連接到 Wi-Fi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi 連接成功");
  Serial.print("分配的 IP 位址: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.print("正在連到MQTT服務氣: ");
  Serial.println(mqttServer);

  while (!client.connected()) {
    if (client.connect("ESPduinoClient")) {  
      Serial.println("MQTT 連接成功");

     
      client.subscribe("sensor/weight");
    } else {
      Serial.print("MQTT 連接失敗，狀態碼 = ");
      Serial.println(client.state());
      delay(5000); 
    }
  }
}
