


var mqtt = require("tinyMQTT").create("m24.cloudmqtt.com", {
    clientId: "tdevice",
    username: "jriiypoc",
    password: "MGZl8VTTlcuc",
    port: 10369 
});

mqtt.on("connected", function(){
    console.log("on"); 
    mqtt.subscribe("/teplica");
    mqtt.subscribe("/temp");
    mqtt.publish("/teplica","Теплица-сервер подключилась к брокеру MQTT-сообщений"); 
});


mqtt.on("published", function(){
    console.log("message sent");
});

mqtt.on("disconnected", function(){
    console.log("disconnected");
});




var wifi = require("Wifi");
var clients = [];

//var WIFI_NAME = "cisco.irk.ru";
//var WIFI_OPTIONS = { password : "Ve!c0dinC1n@" };

var WIFI_NAME = "vaduga";
var WIFI_OPTIONS = { password : "9501203230" };


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
var btn = 'ON';
var ledState = true;
var dht = require("DHT22").connect(NodeMCU.D7);

pinMode(NodeMCU.D4, 'input_pullup'); // для кнопки


// Incoming message handler
function msHandler (msg) {
    console.log(msg.topic);
    console.log(msg.message);

    ledState = ledState ? false : true;  
//    digitalWrite(D2, ledState); // system LED
    digitalWrite(NodeMCU.D3, !ledState); // beeper + lamp
switch (msg.topic) {
case "/temp":
 if (msg.message=="get") {sendTemp();}
    break;
  
default:
    break;
                 }
}
    
mqtt.on("message", msHandler);
  

// Watch for button events (rising and falling)
setWatch(evt => {
sendTemp();
//mqtt.publish("/teplica", btn);
//btn =  btn == 'OFF' ? 'ON' : 'OFF';
}, NodeMCU.D4, {repeat: true, edge: 'falling', debounce: 50});

function sendTemp () {
  dht.read( (a)=> mqtt.publish("/temp", a.temp.toString())  );
}
