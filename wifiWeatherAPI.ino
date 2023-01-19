#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

//https://microcontrollerslab.com/http-get-esp8266-nodemcu-openweathermap-thingspeak/

const char* ssid = "Murugesan";
const char* password = "dgm@2021";

String my_Api_Key = "74128aa6c0432e40529c66508198b4fe";

String my_city = "Chennai"; //specify your city
String my_country_code = "PK"; //specify your country code

unsigned long last_time = 0;
unsigned long timer_delay = 10000;

uint8_t pin_led = 16;

String json_array;
WiFiClient wifiClient;
ESP8266WebServer server;
void setup() {
  Serial.begin(115200);
  pinMode(pin_led,OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WIFI...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/toggle",toggleLED);
  server.on("/pantilt",setPanTilt);
  server.begin();

  Serial.println("First set of readings will appear after 10 seconds");
}

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
  server.send(204,"");
}

void setPanTilt()
{
  String data = server.arg("plain");
  Serial.println(data);
  String jBuffer;
  JSONVar jObject = JSON.parse(data);
  String pan = jObject["pan"];
  String tilt = jObject["tilt"];
  Serial.println(pan);
  Serial.println(tilt);
  server.send(204,"");
}

void loop() {
  
  server.handleClient();
  
    // if(WiFi.status()== WL_CONNECTED){
    //   String server = "http://api.openweathermap.org/data/2.5/weather?q=" + my_city + "&appid=" + my_Api_Key;
      
    //   json_array = GET_Request(server.c_str());
    //   Serial.println(json_array);
    //   JSONVar my_obj = JSON.parse(json_array);
  
    //   if (JSON.typeof(my_obj) == "undefined") {
    //     Serial.println("Parsing input failed!");
    //     return;
    //   }
    
    //   Serial.print("JSON object = ");
    //   Serial.println(my_obj);
    //   Serial.print("Temperature: ");
    //   Serial.println(my_obj["main"]["temp"]);
    //   Serial.print("Pressure: ");
    //   Serial.println(my_obj["main"]["pressure"]);
    //   Serial.print("Humidity: ");
    //   Serial.println(my_obj["main"]["humidity"]);
    //   Serial.print("Wind Speed: ");
    //   Serial.println(my_obj["wind"]["speed"]);
    //   delay(5000);
    // }
    // else {
    //   Serial.println("WiFi Disconnected");
    // }
    // last_time = millis();
}

String GET_Request(const char* server) {
  HTTPClient http;    
  http.begin(wifiClient,server);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}