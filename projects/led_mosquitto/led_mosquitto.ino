#include <ESP8266WiFi.h>  // эта библиотека идет "в коробке" когда устанавливаешь поддержку esp8266 в arduino ide
#include <PubSubClient.h> // эта ставится через "подключить библиотеку -> управлять библиотеками
 
// Connect to the WiFi
const char* ssid = "cisco.irk.ru";        // wifi сеть
const char* password = "Ve!c0dinC1n@";  // wifi пароль
const char* mqtt_server = "postman.cloudmqtt.com";
const char* mqtt_user = "ssngfoep"  ;    
const char* mqtt_password = "vwDusVGhMpRe" ; 
char* ledStatus = "not set";


 
WiFiClient espClient;
PubSubClient client(espClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // обнуляем массив чтобы не приходило лишнее. Называется null terminator. 
  String msg = (char *)payload;  // переносим из указателей в свои переменные. Смотреть тему "указатели" - самое сложное в языке С, про работу с памятью.
  String Topic = topic;    // переносим из указателей в свои переменные. 
  Serial.print("Message arrived [");
  Serial.print(Topic);
  Serial.print(" ");
  Serial.print(msg);
  Serial.print("] ");
  
    if (Topic == "ledStatus/change" && msg == "0")
    
    // у ESP8266 значения на пине встроенного светодиода "перевернуты"
    
   {
   digitalWrite(LED_BUILTIN, HIGH);
   ledStatus = " off";
   }
    
    if (Topic == "ledStatus/change" && msg == "1")
      {
   digitalWrite(LED_BUILTIN, LOW);
   ledStatus = " on";
   }
      
  
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266 Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // ... and subscribe to topic
      client.subscribe("ledStatus");
      client.subscribe("ledStatus/change");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
 Serial.begin(9600);
 
 setup_wifi();
 
 client.setServer(mqtt_server, 12754);  // server port here
 client.setCallback(callback);
 
 pinMode(LED_BUILTIN, OUTPUT);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

long lastMsg = 0;     // для таймера, чтобы отсылать сообщение каждые 2 сек, см. ниже


void loop()
{
 if (!client.connected()) {
  reconnect();
 }



 client.loop();


 long now = millis();
  if (now - lastMsg > 5000) {
  lastMsg = now;
  client.publish("ledStatus", ledStatus); // что то отправляем каждые 5 сек, например статус светодиода
  }
 
 
}

