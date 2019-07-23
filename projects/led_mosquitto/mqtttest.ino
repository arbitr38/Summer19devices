#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <Stepper.h> 

const int stepsPerRevolution = 250; 
Stepper myStepper(stepsPerRevolution, D8,D10,D9,D11);   /// stepsPerRevolution - на сколько шагов поделить полный оборот двигателя.

// bird feeder 

int birdSensorServer = 0; 
int sensor = 0; 
int birdCount = 0; 
int sensorCount = 0; 
int exitCount = 0; 
int birdInside = 0; 
int birdCountFeed = 0; 
int birdFeed = 1.5;//интервал кормления 
int feedWhile = 0; 
const char* birdCountMQTT = "null"; 
// Connect to the WiFi 
//const char* ssid = "vaduga"; // wifi сеть 
//const char* password = "9501203230"; // wifi пароль 

const char* ssid = "cisco.irk.ru"; // wifi сеть 
const char* password = "Ve!c0dinC1n@"; // wifi пароль 
const char* mqtt_server = "postman.cloudmqtt.com"; 
const char* mqtt_user = "ssngfoep" ; 
const char* mqtt_password = "vwDusVGhMpRe"; 



WiFiClient espClient; 
PubSubClient client(espClient); 

void callback(char* topic, byte* payload, unsigned int length) { 
payload[length] = '\0'; // обнуляем массив чтобы не приходило лишнее. Называется null terminator. 
String msg = (char*)payload; // сообщение приходит в бинарном формате (указатель на массив байтов byte*), переводим в массив символов char*, и все пихаем в общий строковый формат String..
String Topic = topic; // массив указателей на символы char* переводим в общий строковый формат для удобного сравнения в if-ах.
Serial.print("Message arrived ["); 
Serial.print(topic); 
Serial.print(" "); 
Serial.print(msg); 
Serial.print("] "); 

if (Topic == "forceFeed")  /// ловим сообщение и поворачиваем на указанное в msg количество шагов, а не в stepPerRevolution. Можем поворачивать назад, если минус-значения приходят.

{ 
Serial.print("повернули на: ");  
Serial.print(msg.toInt());  
Serial.print(" шага(ов)");  
myStepper.step(msg.toInt());      /// stepsPerRevolution - задается сверху один раз и означает на сколько шагов поделить полный оборот двигателя. А тут указываем уже сколько шагов нам сделать за раз
delay (50); 
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

client.subscribe("forceFeed"); 
client.subscribe("countStatus"); 


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
myStepper.setSpeed(40); 

Serial.begin(115200); 
pinMode(A0,INPUT ); //sensor 



setup_wifi(); 

client.setServer(mqtt_server, 12754); // server port here 
client.setCallback(callback); 


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

long lastMsg = 0; // для таймера, чтобы отсылать сообщение каждые 2 сек, см. ниже 


void loop(){ 


if (!client.connected()) { 
reconnect();} 



client.loop(); 
char birdsCountChar[5];                                    /// готовим пустой массив символов под четыре разряда цифр
String(birdCount).toCharArray(birdsCountChar,5);          /// birdcount переводим в String, оттуда в массив символов birdsCountChar, с четырьмя разрядами цифр)

long now = millis(); 
if (now - lastMsg > 5000) { 
lastMsg = now; 
client.publish("countStatus", birdsCountChar ); // отправляем подсчет птиц каждые 5 сек
} 

//bird feeder 
int laserSensor = analogRead(A0); 

if (laserSensor > 200 ){ 
sensor = 1; 
} 
else{ 
sensor = 0; 
} 

if(sensor == 1 && sensorCount == 0 ){ //swich 1.1 
sensorCount = 1; 
Serial.println("swich 1.1"); 
} 

if(sensor == 0 && sensorCount == 1 ){ //swich 1.2 
Serial.println("swich 1.2"); 
sensorCount = 0; 
exitCount = 1; 
} 

if(exitCount == 1 && birdInside == 0 ){//swich 2.1 
exitCount = 0; 
birdInside = 1; 
birdCount = birdCount + 1; 
birdCountFeed = birdCountFeed + 1; 
} 

if(exitCount == 1 && birdInside == 1 ){ //swich 2.2 
exitCount = 0; 
birdInside = 0; 
Serial.println("swich 2.2"); 
} 

if(birdCountFeed > birdFeed){ //swich 3.1 
Serial.println("swich 3.1"); 
birdCountFeed =
 
0; 

//int howmuch = 0;     //комиплятор ругается на то что в комментах. Надо ли это тут еще раз проверять, если есть команда покормить в callback-фукции на входящие сообщения?
//feedWhile = 0; 
//
//while (feedWhile !>= howmuch) { 
//feedWhile = feedWhile + 1 ;
//myStepper.step(stepsPerRevolution); 


delay (50); 
} 
delay(35); 
}
