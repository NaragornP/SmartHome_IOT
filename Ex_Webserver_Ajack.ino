#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "Smart Home";
const char* password = "smart_home";
//const char* ssid = "Bananaboat :)";
//const char* password = "0814095572";

#define LED     D0        // Led in NodeMCU at pin GPIO16 (D0).
#define ch2 D3
const int analogInPin = A0; 
int sensorValue = 0;   
String countLDR ="OFF";     
String countDHT ="";   
int light=0;

#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE,15);
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED, OUTPUT);
  pinMode(ch2, OUTPUT);  
  dht.begin();
  
  Serial.println();
  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  server.begin();
//  Serial.println("Server started");
//  Serial.println(WiFi.localIP());
}

//----- Loop routine. --------------------------
void loop() {
    Serial.print("Re Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  //-----------------------------------------------------
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
//    --------------------------temp-------------------------------        
  if(req.indexOf("/Temp_.html") != -1)  {
    
   sensorValue = analogRead(analogInPin);
     int sensorLDR = sensorValue+45;
     float h = dht.readHumidity();
//     float hh = h+1.0;
     float hh = h;
     float t = dht.readTemperature();
     float tt = t+0.8;
     
      if (sensorLDR<145){
        digitalWrite(LED, HIGH);
        light=1;
        countLDR = "ON";
      }else if (sensorLDR>280){
        digitalWrite(LED, LOW);
        light = 0;     
        countLDR = "OFF";
      }else{
        if(countLDR == "ON"){
          countLDR = "ON";
          }else if (countLDR == "OFF"){
          countLDR = "OFF";
          }
       }
       if (t>30.00){
        digitalWrite(ch2, HIGH);
        countDHT = "ON";
      }else{
        digitalWrite(ch2, LOW);
        countDHT = "OFF"; 
      }


      client.flush();      
      String humi=String(tt)+" "+String(hh)+" "+countDHT+" "+String(sensorLDR)+" "+countLDR+" ";
      client.print(humi);

      
      String show="<h3>Temp = ";
      show += String(tt)+" C</h3>\r\n";
      show += "<h3>Humidity = ";
      show += String(hh)+" %</h3>\r\n";
      show += "<h3>LDR = ";
      show += String(sensorLDR)+"</h3>\r\n";
      show += "<h3>LDR Status = ";
      show += String(countLDR)+"</h3>\r\n";
      show += "<h3>DHT Status = ";
      show += String(countDHT)+"</h3>\r\n";
      client.print(show);

  }
  else if(req.indexOf("/") != -1)
  {
    client.flush();
  
    String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
    web += "<html>\r\n";
    web += "<head>\r\n";
    web += "<meta name='apple-mobile-web-app-capable' content='yes' />";
    web += "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />";
    web += "<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />";
   
    web += "<style>\r\n";
    web += "h1{color:black;font-family:Arial;font-size:40px;text-align:center;}\r\n";	      
    web += "</style>\r\n";
      web += "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\r\n";
//        ----------------------------------Script----UpSheets-------------------
      web += "<script>\r\n";
      web += "sub = [];\r\n";
      web += "function postContactToGoogle() {\r\n";
      web += "$.ajax({\r\n";
      web += "url:\"https://docs.google.com/forms/d/1P2JQrBILegp9mgeRxgaMXB2Fr0oTOBPgHgwlDP6cy5s/formResponse\",\r\n";
      web += "data:{\"entry_234323749\":sub[0],\"entry_77492089\":sub[1],\"entry_1077086998\":sub[2],\"entry_105922511\":sub[3],\"entry_572583771\":sub[4]},\r\n";
      web += "type:\"POST\",\r\n";
      web += "dataType:\"xml\"});\r\n";
      web += "console.log(sub[0]);console.log(sub[1]);console.log(sub[2]);console.log(sub[3]);console.log(sub[4]);}\r\n";
      web += "</script>\r\n";
    web += "<script>\r\n";
    web += "var ajax = null;\r\n";
    web += "if (window.XMLHttpRequest)\r\n";
    web += "{ajax =new XMLHttpRequest();}\r\n";
    web += "else\r\n";
    web += "{ajax=new ActiveXObject(\"Microsoft.XMLHTTP\");}\r\n";
    web += "function ajaxLoad(method ,URL,displayElementId,sendData)\r\n";
    web += "{\r\n";
    web += "if(!ajax){alert(\"not support\");return;}\r\n";
    web += "ajax.open(method,URL,true);\r\n";
    web += "ajax.onreadystatechange = function()\r\n";
    web += "{\r\n";
    web += "if(ajax.readyState == 4 && ajax.status==200)\r\n";
    web += "{\r\n";
    web += "var ajax_result = ajax.responseText;\r\n";
    web += "var el = document.getElementById(displayElementId);\r\n";
    web += "el.innerHTML = ajax_result;\r\n";
    web += "var str = ajax_result;\r\n";
//    sprint
    web += "sub = str.split(\" \");\r\n";
//    log
//      web += "console.log(sub[0]);console.log(sub[1]);console.log(sub[2]);\r\n";
    web += "}\r\n";
    web += "}\r\n";	
    web += "ajax.send(sendData);\r\n";
    web += "}\r\n";
    web += "function update_temp_humi()\r\n"; 
    web += "{\r\n";
    web += "var rand = Math.random();\r\n";
    web += "var URL = \"/Temp_.html\";\r\n";
    web += "ajaxLoad(\"GET\",URL,\'temp_humi\',null);\r\n";
    web += "}\r\n";
    web += "</script>\r\n";
    web += "</head>";
    web += "<body style=\"text-align:center\">";
    
    web += "<h1>Smart Home</h1>"; 
    web += "<h2>Temp & Humidity & LDR</h2>";
    
//    web += "<font size=\"30\">";
    web += "<div id=\"temp_humi\">";
    web += "<h3>Loading ...</h3>";
//    web += "</font>";
    
    web += "</div>";
//    web += "<button onclick=\"postContactToGoogle()\">Click me</button>";
      web += "<hr />\r\n";

      web += "<br />\r\n";
    web += "<script>";
    web += "function Relay(state){ajaxLoad(\"GET\",state+\'.html\',null,null);}";	
    web += "setInterval(\"update_temp_humi()\",5000);";
    web += "setInterval(\"postContactToGoogle()\",600000);";//15นาที อัพกูเกิล
    web += "</script>";
    web += "</body>";
    web += "</html>";
     client.print(web);
    return;
  }
  
}

