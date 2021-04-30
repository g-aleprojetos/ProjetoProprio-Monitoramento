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

// --- Protótipo das Funções ---
boolean configRead();
boolean configSave();
void  configReset();
void handleHomeInit();
void handleConfigInit();
void handleCSS();

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

void setup() {
 
  Serial.begin(9600);          // Velocidade para ESP8266

  Serial.println("Entrou no setup\n ");
 
   if (!SPIFFS.begin()) {        // SPIFFS
      Serial.println("SPIFFS ERRO");
    while (true);
  }

  configRead();                 // Lê configuração
  configSave();                 // Salva configuração

  pinMode(LED_PIN, OUTPUT);     // LED
 
  //--- WiFi Station ---
 Serial.println("Entrou no WiFi\n ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);
  Serial.println("Conectando WiFi ");
  int b = 31;
  while(WiFi.status() != WL_CONNECTED && b > 0) {
    b--;
    Serial.print(b);
    Serial.print(" ");
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
   
  Serial.println("Entrou em configurar server\n ");
  server.on(F("/configInit")    , handleConfigInit);
  server.on(F("/css")       , handleCSS);
  server.onNotFound(handleHomeInit);
  server.collectHeaders(WEBSERVER_HEADER_KEYS, 1);
  server.begin();
  }
}

void loop() {
  yield();                            // WatchDog
  dnsServer.processNextRequest();     // DNS 
  server.handleClient();              // Web 
}

// Funções Genéricas ------------------------------------

void log(String s) {
  // Gera log na Serial
  Serial.println(s);
}

String ipStr(const IPAddress &ip) {
  // Retorna IPAddress em formato "n.n.n.n"
  String sFn = "";
  for (byte bFn = 0; bFn < 3; bFn++) {
    sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
  }
  sFn += String(((ip >> 8 * 3)) & 0xFF);
  return sFn;
}

// Funções de Configuração ------------------------------

void  configReset() {
  // Define configuração padrão
 strlcpy(login, "admin", sizeof(login));
 strlcpy(senha, "admin", sizeof(senha));
 strlcpy(ssid, "", sizeof(ssid)); 
 strlcpy(pw, "", sizeof(pw)); 
  
}
boolean configRead() {
  // Lê configuração
  StaticJsonDocument<256> jsonConfigInit;

  File file = SPIFFS.open(F("/ConfigInit.json"), "r");
  if (deserializeJson(jsonConfigInit, file)) {
    // Falha na leitura, assume valores padrão
    configReset();

    log(F("Falha lendo ConfigInit, assumindo valores padrão."));
    return false;
  } else {
    // Sucesso na leitura
     strlcpy(login, jsonConfigInit["login"]        | "", sizeof(login));
     strlcpy(senha,  jsonConfigInit["senha"]       | "", sizeof(senha));
     strlcpy(ssid, jsonConfigInit["ssid"]          | "", sizeof(ssid)); 
     strlcpy(pw, jsonConfigInit["pw"]              | "", sizeof(pw)); 

    file.close();

    Serial.println("\nLendo config:");
    serializeJsonPretty(jsonConfigInit, Serial);
    Serial.println("Saindo da leitura da configuração");
    Serial.println("");

    return true;
  }
}

boolean configSave() {
  // Grava configuração
   StaticJsonDocument<256> jsonConfigInit;

  File file = SPIFFS.open(F("/ConfigInit.json"), "w+");
  if (file) {
    // Atribui valores ao JSON e grava
    jsonConfigInit["login"]    = login;
    jsonConfigInit["senha"]    = senha;
    jsonConfigInit["ssid"]     = ssid;
    jsonConfigInit["pw"]       = pw;
  
    serializeJsonPretty(jsonConfigInit, file);
    file.close();

    log(F("\nGravando config:"));
    serializeJsonPretty(jsonConfigInit, Serial);
    Serial.println("Saindo do configSave");
    log("");

    return true;
  }
  return false;
}

// Requisições Web --------------------------------------
void handleHomeInit() {
  // Home
  File file = SPIFFS.open(F("/ConfigInit.html"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    s.replace(F("#login#")     , login);
    s.replace(F("#senha#")     , senha);
    s.replace(F("#ssid#")      , ssid);
      
    // Envia dados
    server.send(200, F("text/html"), s);
    log("Home Config - Cliente: " + ipStr(server.client().remoteIP()) +
        (server.uri() != "/" ? " [" + server.uri() + "]" : ""));
  } else {
    server.send(500, F("text/plain"), F("Home - ERROR 500"));
    log(F("Home Config - ERRO lendo arquivo"));
  }
}

void handleConfigInit() {
  // Config
  File file = SPIFFS.open(F("/ConfigInit.html"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    s.replace(F("#login#")     , login);
    s.replace(F("#senha#")     , senha);
    s.replace(F("#ssid#")      , ssid);

    // Send data
    server.send(200, F("text/html"), s);
    log("Config - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, F("text/plain"), F("Config - ERROR 500"));
    log(F("ConfigInit - ERRO lendo arquivo"));
  }
}

void handleCSS() {
  // Arquivo CSS
  File file = SPIFFS.open(F("/Style.css"), "r");
  if (file) {
    // Define cache para 3 dias
    server.sendHeader(F("Cache-Control"), F("public, max-age=172800"));
    server.streamFile(file, F("text/css"));
    file.close();
    log("CSS - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, F("text/plain"), F("CSS - ERROR 500"));
    log(F("CSS - ERRO lendo arquivo"));
  }
}
