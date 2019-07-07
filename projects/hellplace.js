var mqtt = require("tinyMQTT").create("postman.cloudmqtt.com", {
    clientId: "kdevice",
    username: "ssngfoep",
    password: "vwDusVGhMpRe",
    port: 12754 
});

mqtt.on("connected", function(){
    console.log("on"); 
  mqtt.subscribe("/feeder/get");
  mqtt.subscribe("/feeder/count");
  mqtt.subscribe("/temp/get");
  mqtt.subscribe("/door/turn");
  
    mqtt.publish("/feeder","Кормушка подключилась к MQTT"); 
});


mqtt.on("published", function(){
    console.log("message sent");
});

mqtt.on("disconnected", function(){
    console.log("disconnected");
});




var wifi = require("Wifi");

var WIFI_NAME = "cisco.irk.ru";
var WIFI_OPTIONS = { password : "Ve!c0dinC1n@" };

//var WIFI_NAME = "vaduga";
//var WIFI_OPTIONS = { password : "9501203230" };


wifi.connect(WIFI_NAME, WIFI_OPTIONS, function(err) {
  if (err) {
    console.log("Connection error: "+err);
    return;
  }
  console.log("Wi-Fi connected!");
  mqtt.connect();
});


 // Print IP address
  wifi.getIP((err, info) => {
    if (err !== null) {
      throw err;
    }
   print("http://"+info.ip);
        
  });


/////////////////////////////////////

var dht = require("DHT22").connect(NodeMCU.D1);  // датчик температуры
var StepperMotor = require("StepperMotor"); /// шаговый мотор для шнека (сыпать корм)

var motor = new StepperMotor({
  pins:[NodeMCU.D5,NodeMCU.D6,NodeMCU.D7, NodeMCU.D8]
});


var timer = 0;                              ///не решена задача подсчета птиц. пин А0 считывает либо сильно быстро, либо медленно. Ложные срабатывания. Надо продумать логику опрашивания пина и как считать.
digitalWrite(NodeMCU.D3, 1);
var birds = 0;

function alarma() {
  digitalWrite(NodeMCU.D2, analogRead(A0) > 0.3);
  if (analogRead(A0) < 0.3) {timer++;}
  if (timer > 0 & timer % 5 === 0) {birds++;}
  
}

var count = 0;

function corn() {
  if (count < birds & birds % 10 === 0) {
    motor.moveTo(motor.getPosition()+400); 
    count = birds;
                                        }
                }
  

setInterval(alarma, 30);
setInterval(corn, 150);



var door = false;

// Incoming message handler

function msHandler (msg) {
    console.log(msg.topic);
    console.log(msg.message);

    digitalWrite(D2, false);
  setTimeout("digitalWrite(D2, true)",500); // system LED
// digitalWrite(NodeMCU.D3, !ledState); 
switch (msg.topic) {

case "/feeder/get":
 mqtt.publish("/feeder/count",birds.toString()); 
    break;
case "/temp/get":
 dht.read( (a)=> mqtt.publish("/temp", a.temp.toString())  );
    break;
case "/door/turn":
    
 if (door === false) {motor.moveTo(motor.getPosition()+400);} 
 else motor.moveTo(motor.getPosition()-400); 
    door = !door;
    
    
    
    break;
default:
    break;
                 }
}

mqtt.on("message", msHandler);
    







