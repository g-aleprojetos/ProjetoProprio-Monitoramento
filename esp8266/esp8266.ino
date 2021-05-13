 /*
 *  AUTOR: Alexandre Gonçalves
 *  EMAIL:g.aleprojetos@gmail.com
 *  ARQUIVO:dispositivo_esp8266.ino
 *  DATA:12/05/2021 upgrade 
 *  
 */

 // --- Biblioteca ---
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <FS.h>
#include <ArduinoJson.h>

 
const char* ssid = "g.aleprojetos";
const char* password = "a248314b";
String buf;
 
WiFiServer server(80);
 
 
void setup(void){
  //Configura a porta serial para 115200bps
  Serial.begin(115200);
 
  //Abre o sistema de arquivos (mount)
  openFS();
  //Cria o arquivo caso o mesmo não exista
  createFile();
 
  writeFile("Booting ESP8266...");
  writeFile("Connecting to " + (String)ssid);
 
  //Inicia a conexão WiFi
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  writeFile("WiFi connected");
  Serial.println(WiFi.localIP());
 
  //Inicia o webserver
  server.begin();
  writeFile("Web Server started");
}
 
void loop(void){
 
  //Tratamento das requisições http
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
 
  buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<h3 style=""text-align: center;"">ESP8266 Web Log</h3>";
  buf += "<p>";
  readFile();
  buf += "</p>";
  buf += "</html>\n";
 
  client.print(buf);
  client.flush();
 
  Serial.println("Client disonnected");
}
