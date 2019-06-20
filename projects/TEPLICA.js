
var mqtt = require("tinyMQTT").create("m24.cloudmqtt.com", {
    clientId: "tdevice",
    username: "jriiypoc",
    password: "MGZl8VTTlcuc",
    port: 10369 
});

mqtt.on("connected", function(){
    console.log("on"); 
    mqtt.subscribe("/temp/get");
    mqtt.subscribe("/door/turn");
    mqtt.subscribe("/relay/turn");
    mqtt.subscribe("/humidity/get");
    mqtt.publish("/teplica","Теплица подключилась к брокеру MQTT-сообщений"); 
});


mqtt.on("published", function(){
    console.log("message sent");
});

mqtt.on("disconnected", function(){
    console.log("disconnected");
});




var wifi = require("Wifi");
var clients = [];

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




//// its mine
var moistureSensor = require('moisturesensor').connect(NodeMCU.A0);
var dht = require("DHT22").connect(NodeMCU.D7);
var relay = 0;

pinMode(NodeMCU.D6, 'input_pullup'); // для кнопки


// Incoming message handler
function msHandler (msg) {
    console.log(msg.topic);
    console.log(msg.message);

    digitalWrite(D2, false);
  setTimeout("digitalWrite(D2, true)",500); // system LED
// digitalWrite(NodeMCU.D3, !ledState); 
switch (msg.topic) {
case "/temp/get":
 sendTemp();
    break;
case "/door/turn":
 if (isNaN(parseFloat(msg.message))===false) {
   s.move(msg.message, 5000);
   mqtt.publish("/door", "at position "+ msg.message);
                }
    break;
    
case "/relay/turn":
    relay = relay === 0? 1:0;
    digitalWrite(NodeMCU.D3, relay);
    mqtt.publish("/relay", "at position "+ relay);
    break;
case "/humidity/get":
    mqtt.publish("/humidity", moistureSensor.getMoistureLevel().toString());
         break;
  
default:
    break;
                 }
}
    
mqtt.on("message", msHandler);
  

// Watch for button events (rising and falling)
var door = 0.05;
var s = require("servo").connect(NodeMCU.D1);

setWatch(evt => {
door = door==0.05 ? 1 : 0.05;
s.move(door, 5000); 
mqtt.publish("/door", door==0.05 ? "closed" : "opened");
}, NodeMCU.D6, {repeat: true, edge: 'falling', debounce: 50});

function sendTemp () {
  dht.read( (a)=> mqtt.publish("/temp", a.temp.toString())  );
}
