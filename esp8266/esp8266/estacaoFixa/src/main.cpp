/*
 *  AUTOR: Alexandre Gonçalves
 *  EMAIL:g.aleprojetos@gmail.com
 *  ARQUIVO:dispositivo_esp8266.ino
 *  DATA:28/03/2021 upgrade 
 *  
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <FS.h>
#include <ArduinoJson.h>

// --- Configuração da rede e dispositivo ---
const byte        WEBSERVER_PORT          = 80;                 // Porta Servidor Web
const char*       WEBSERVER_HEADER_KEYS[] = {"User-Agent"};     // Headers do Servidor Web
const byte        DNSSERVER_PORT          = 53;                 // Porta Servidor DNS
const byte        LED_PIN                 = LED_BUILTIN;        // ESP8266 possui pino padrão de LED
String            apWifi                  = "Wifi Automação";   // Nome do Wifi

//--- Tamanho do Objeto JSON ---
const   size_t    JSON_SIZE               = JSON_OBJECT_SIZE(5) + 600;

//--- Instâncias --- 
ESP8266WebServer  server(WEBSERVER_PORT);                         // Web Server
DNSServer         dnsServer;                                      // DNS Server
IPAddress         apIP(192, 168, 4, 1);

//--- Variáveis Globais ---
char              login[30];    // login
char              senha[30];    // senha
char              id[30];       // Identificação do dispositivo
char              ssid[30];     // Rede WiFi
char              pw[30];       // Senha da Rede WiFi
char              masc[15];     // Marcara 
char              gat[15];      // gateway
word              bootCount;    // Número de inicializações
boolean           ledOn;        // Estado do LED
char              idSensor[30]; // Identificação do sensor
int               sensor;       // sensor

String responseHTML = ""
                      "<!DOCTYPE html><html lang='en'><head>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>Wifi Automação</title></head><body>"
                      "<h1>Hello World!</h1><p>Rede de automação."
                      " All requests will be redirected here.</p></body></html>";

void setup() {
 
  Serial.begin(9600);          // Velocidade para ESP8266
 
 /*  if (!SPIFFS.begin()) {        // SPIFFS
      Serial.println("SPIFFS ERRO");
    while (true);
  }*/

  pinMode(LED_PIN, OUTPUT);     // LED
 
  //--- WiFi Station ---
  WiFi.begin(ssid, pw);
  Serial.println("Conectando WiFi ");
  byte b = 0;
  while(WiFi.status() != WL_CONNECTED && b < 60) {
    b++;
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    
    // WiFi Station conectado
    Serial.println("WiFi conectado");

  } else {
    
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP( apWifi );

     //--- Habilita roteamento DNS ---
   dnsServer.start(DNSSERVER_PORT , "*", apIP);
   dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
   
  // replay to all requests with same HTML
  server.onNotFound([]() {
  server.send(200, "text/html", responseHTML);
  });
  server.begin();
  }
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

// Funções Genéricas ------------------------------------
