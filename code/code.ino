#include "WiFi.h"
#include "HTTPClient.h"
#include <WiFiClient.h> 
#include <WebServer.h>

char* wifi_ssid = "manish";
char* wifi_pwd = "Horcrux123";
String cse_ip = "onem2m.iiit.ac.in";
////////////////////////////////////////
//Web address to read from
const char *host = "api.thingspeak.com";
String apiKey = "29NLXUYZLYSV1DPG";  //ENTER YOUR API KEY <<<<<<<<<<<

String cse_port = "443";
String server = "http://"+cse_ip+":"+cse_port+"/~/in-cse/in-name/";

String createCI(String server, String ae, String cnt, String val)
{
  HTTPClient http;
  http.begin(server + ae + "/" + cnt + "/");
  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");
  http.addHeader("Connection", "close");
  int code = http.POST("{\"m2m:cin\": {\"cnf\": \"text/plain:0\",\"con\": "+ String(val) +"}}");
  Serial.println(code);
  http.end();
  delay(300);
  return "success";
}

void setup()
{
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(wifi_ssid, wifi_pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  //LED ON CODE INDICATING CONN TO WIFI

  Serial.println("Setup done");
}

void loop()
{
  WiFiClient client;          
  const int httpPort = 80; //Port 80 is commonly used for www

  //Connect to host, host(web site) is define at top 
  if(!client.connect(host, httpPort)){
   Serial.println("Connection Failed");
   delay(300);
   return; //Keep retrying until we get connected
  }
  
  int sensor1 = analogRead(34);
  Serial.print("Moisture Sensor1: ");
  Serial.print(sensor1);
  Serial.println("");
  int sensor2 = analogRead(32);
  Serial.print("Moisture Sensor2: ");
  Serial.print(sensor2);
  Serial.println("");
  int sensor3 = analogRead(33);
  Serial.print("Moisture Sensor3: ");
  Serial.print(sensor3);
  Serial.println("");
  int sensor4 = analogRead(35);
  Serial.print("Moisture Sensor4: ");
  Serial.print(sensor4);
  Serial.println("");

  // when single sensor gives multiple values
  String sensor = String(sensor1) + "," + String(sensor2) + "," + String(sensor3) + "," + String(sensor4); 

  // Make it OneM2M complaint
  String sensor_string = "\"" + sensor + "\""; // DO NOT CHANGE THIS LINE

  
  String ADCData;
  float adcvalue = sensor1;  //Read Analog value of LDR
  ADCData = String(adcvalue);   //String to interger conversion
  String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
  Link = Link + ADCData;
  adcvalue = sensor2;
  ADCData = String(adcvalue);
  Link = Link + "&field2=";
  Link = Link + ADCData;
  adcvalue = sensor3;
  ADCData = String(adcvalue);
  Link = Link + "&field3=";
  Link = Link + ADCData;
  adcvalue = sensor4;
  ADCData = String(adcvalue);
  Link = Link + "&field4=";
  Link = Link + ADCData;
  Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                

  // send to things speak
  client.print(Link);

  // Send data to OneM2M server
  createCI(server, "Team37_Soil_Moisture_Sensing", "node_1", sensor_string);

  delay(500000); // DO NOT CHANGE THIS LINE
}
