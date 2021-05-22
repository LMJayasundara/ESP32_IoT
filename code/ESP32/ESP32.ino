#include "WiFiConfig.h" // WiFi configuration
#include <WiFi.h> // ESP32 WiFi include
#include <PubSubClient.h>

const char* mqtt_server = "18.130.164.228"; // YOUR_MQTT_BROKER_IP_ADDRESS
const char* mqttUser = "admin";
const char* mqttPassword = "admin@123";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

float temperature = 0;
float heart_rate = 0;
float gas = 0;

void ConnectToWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting to "); Serial.println(SSID);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
  
  ConnectToWiFi();
  client.setServer(mqtt_server, 1883);
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    
    temperature = random(10, 30);
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);

    heart_rate = random(70, 80);
    
    // Convert the value to a char array
    char heart_rateString[8];
    dtostrf(heart_rate, 1, 2, heart_rateString);
    Serial.print("Heart Rate: ");
    Serial.println(heart_rateString);
    client.publish("esp32/heart_rate", heart_rateString);

    gas = random(0, 1);
    
    // Convert the value to a char array
    char gasString[8];
    dtostrf(gas, 1, 2, gasString);
    Serial.print("Gas: ");
    Serial.println(gasString);
    client.publish("esp32/gas", gasString);
    
  }
}
