#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#define LED_PIN D4

ESP8266WebServer server(80);

// WiFi AP Config
IPAddress localIp(192,168,1,1);
IPAddress gatewayIp(192,168,1,1);
IPAddress subnet(255,255,255,0);
const char *ssid="esp8266_swa";
const char *pwd="swa12345678";

// BME280
Adafruit_BME280 bme;
float temperature;
float humidity;
float pressure;

void handle_home_page() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  char body[1024];
  sprintf(body,  "<html> <head>   <title>ESP8266 Page</title> <meta name='viewport' content='width=device-width, initial-scale=1.0'>  <style>     h1 {text-align:center; }     td {font-size: 50%; padding-top: 30px;}     .temp {font-size:150%; color: #FF0000;}     .press {font-size:150%; color: #00FF00;}     .hum {font-size:150%; color: #0000FF;}   </style> </head>  <body>    <h1>ESP8266 Sensor Page</h1>    <div id='div1'>        <table>           <tr>            <td>Temperature</td><td class='temp'>%.2f</td>          </tr>          <tr>           <td>Pressure</td><td class='press'>%.2f</td>          </tr>          <tr>            <td>Humidity</td><td class='hum'>%.2f</td>  </tr>    </div> </body>  </html>", bme.readTemperature(), (bme.readPressure() / 100.0F), bme.readHumidity());
  server.send(200, "text/html", body);
}

void handle_led_on() {
  digitalWrite(LED_PIN, HIGH);
}

void handle_led_off() {
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  // Setup Wifi
  Serial.begin(9600);
  Serial.println("Initializing AP...");
  if (WiFi.softAP(ssid, pwd)) {
    Serial.println("AP ready....");
    WiFi.softAPConfig(localIp, gatewayIp, subnet);
    delay(200);
 
  }
  Serial.println("Wifi AP ready...");
  bme.begin(0x76);
  //bme.begin();
  server.on("/", handle_home_page);
  server.on("/led/on", handle_led_on);
  server.on("/led/off", handle_led_off);
  server.begin();

   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);
     

}

void loop() {
  server.handleClient();

}