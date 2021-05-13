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

 // --- Configuração da rede e dispositivo ---
const char* ssid = "g.aleprojetos";
const char* password = "a248314b";
WiFiServer server(80);
 
 // --- Protótipo das Funções ---
void openFS();
void createFile();
void writeFile(String msg);
void readFile(void);

//--- Variáveis Globais ---
String buf;
 
void setup(){
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
 
void loop(){
 
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
 
  Serial.println("Client disconnected");
}


// Funções de Configuração ------------------------------

void openFS(){
  //Abre o sistema de arquivos
  if(!SPIFFS.begin()){
    Serial.println("Erro ao abrir o sistema de arquivos");
  } else {
    Serial.println("Sistema de arquivos aberto com sucesso!");
  }
}

void createFile(){
  File wFile;
 
  //Cria o arquivo se ele não existir
    if(SPIFFS.exists("/log.txt")){
    Serial.println("Arquivo ja existe!");
    } else {
    Serial.println("Criando o arquivo...");
    wFile = SPIFFS.open("/log.txt","w+");
 
    //Verifica a criação do arquivo
    if(!wFile){
      Serial.println("Erro ao criar arquivo!");
    } else {
      Serial.println("Arquivo criado com sucesso!");
    }
  }
  wFile.close();
}

void writeFile(String msg) {
 
  //Abre o arquivo para adição (append)
  //Inclue sempre a escrita na ultima linha do arquivo
  File rFile = SPIFFS.open("/log.txt","a+");
 
  if(!rFile){
    Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.println("Log: " + msg);
    Serial.println(msg);
  }
  rFile.close();
}

void readFile() {
  //Faz a leitura do arquivo
  File rFile = SPIFFS.open("/log.txt","r");
  Serial.println("Reading file...");
  while(rFile.available()) {
    String line = rFile.readStringUntil('\n');
    buf += line;
    buf += "<br />";
  }
  rFile.close();
}
