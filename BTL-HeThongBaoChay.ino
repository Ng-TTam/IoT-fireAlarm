#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>

#define FIRE_SENSOR_PIN 23
#define LED_PIN 5
#define BUZZER_PIN 4
#define PUMP_PIN 22

const char* ssid = "Galaxy A117607";
const char* password = "rrrrrrrr";
const char* mqtt_server = "broker.hivemq.com";
String latitude = "21.0285";
String longitude = "105.8542";
String coordinates = latitude + "," + longitude;//run web first

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
bool isAutoMode = true;
bool dataSend = false;

void reconnect() { 
  while (!client.connected()) {// Loop until we're reconnected
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to " + clientId);
      client.publish("/BTL/IoT/fireAlarm/coordinates", coordinates.c_str());
      dataSend = true;
      client.subscribe("/BTL/IoT/fireAlarm/coordinates");
      client.subscribe("/BTL/IoT/fireAlarm/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "/BTL/IoT/fireAlarm/control") == 0) {
    String messageOrigin((char*)payload, length);
    std::stringstream ss(messageOrigin.c_str());
    std::string token;
    std::string messageTok[3];
    int index = 0;

    while (std::getline(ss, token, ',')) {
      messageTok[index++] = token;
    }
    String messageLat = String(messageTok[0].c_str());
    String messageLng = String(messageTok[1].c_str());
    String message = String(messageTok[2].c_str());

    if(messageLat == latitude && messageLng == longitude){
      if (message == "auto") {
        isAutoMode = true;
        Serial.println("Received 'auto' command. auto mode turned on.");
      } else if (message == "control") {
        isAutoMode = false;
        Serial.println("Received 'control' command. control mode turned on.");
      } else if (!isAutoMode && message == "led_on") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Received 'led_on' command. LED turned on.");
      } else if (!isAutoMode && message == "led_off") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Received 'led_off' command. LED turned off.");
      } else if (!isAutoMode && message == "buzzer_on") {
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("Received 'buzzer_on' command. BUZZER turned on.");
      } else if (!isAutoMode && message == "buzzer_off") {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Received 'buzzer_off' command. BUZZER turned off.");
      } else if (!isAutoMode && message == "pump_on") {
        digitalWrite(PUMP_PIN, HIGH);
        Serial.println("Received 'pump_on' command. PUMP turned on.");
      } else if (!isAutoMode && message == "pump_off") {
        digitalWrite(PUMP_PIN, LOW);
        Serial.println("Received 'pump_off' command. PUMP turned off.");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
  pinMode(FIRE_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

void loop() { 
  int fireStatus = digitalRead(FIRE_SENSOR_PIN);// Đọc trạng thái cảm biến lửa
  int ledStatus = digitalRead(LED_PIN);
  int buzzerStatus = digitalRead(BUZZER_PIN);
  int pumpStatus = digitalRead(PUMP_PIN);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(!dataSend){ 
    client.publish("/BTL/IoT/fireAlarm/coordinates", coordinates.c_str());
    dataSend = true;
  }

  unsigned long now = millis();
  if (now - lastMsg > 2000) { 
    lastMsg = now;
  }

  if(isAutoMode){
    if (fireStatus == LOW) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(PUMP_PIN, HIGH);
      Serial.println("Fire detected!");
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(PUMP_PIN, LOW);
    }
  }

  if(isAutoMode && fireStatus == LOW){
    String mess = coordinates +",fire";
    client.publish("/BTL/IoT/fireAlarm/location", mess.c_str());
  }else{
    String mess = coordinates;
    if(ledStatus == HIGH) mess.concat(",led_on"); else mess.concat(",led_off");
    if(buzzerStatus == HIGH) mess.concat(",buzzer_on"); else mess.concat(",buzzer_off");
    if(pumpStatus == HIGH) mess.concat(",pump_on"); else mess.concat(",pump_off");
    client.publish("/BTL/IoT/fireAlarm/location", mess.c_str());
  }
  delay(1000);
}
