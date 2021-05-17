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
void formatFS();
void deleteFile();
void openFS();
void createFileConfInit();
boolean configRead();
void  configReset();
void readFile();
boolean configSave();
String deviceID();

//--- Variáveis Globais ---
char              login[30];    // login
char              senha[30];    // senha
char                 id[30];    // Identificação do dispositivo
char               ssid[30];    // Rede WiFi
char                 pw[30];    // Senha da Rede WiFi
String buf;
 
void setup(){
  //Configura a porta serial para 115200bps
  Serial.begin(9600);

   //Abre o sistema de arquivos (mount)
  openFS();
  
  //formatFS();
  //deleteFile();
 
  //Cria o arquivo caso o mesmo não exista
  createFileConfInit();
  //lê o arquivo de configuração
  configRead();

  pinMode(LED_PIN, OUTPUT);     // LED
  digitalWrite(LED_PIN, HIGH); 
 
  //--- WiFi Station ---
 
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
 
  Serial.println(WiFi.localIP());
  server.on(F("/login")    , handleLogin);
  server.on(F("/configInit")    , handleConfigInit);
  server.on(F("/configSave"), handleConfigSave);
  server.on(F("/css")       , handleCSS);
  server.onNotFound(handleLogin);
  server.collectHeaders(WEBSERVER_HEADER_KEYS, 1);
  Serial.println("Entrou em configurar server\n ");

  } else {
    
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP( apWifi );

     //--- Habilita roteamento DNS ---
  dnsServer.start(DNSSERVER_PORT , "*", apIP);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  server.on(F("/configInit")    , handleConfigInit);
  server.on(F("/configSave"), handleConfigSave);
  server.on(F("/css")       , handleCSS);
  server.onNotFound(handleConfigInit);
  server.collectHeaders(WEBSERVER_HEADER_KEYS, 1);
  Serial.println("Entrou em configurar server\n ");

   }

   server.begin();
}
 
void loop(){
  yield();                            // WatchDog
  dnsServer.processNextRequest();     // DNS 
  server.handleClient();              // Web 

}

// Funções Genéricas ------------------------------------
void log(String s) {
  // Gera log na Serial
  Serial.println(s);
}

String softwareStr() {
  // Retorna nome do software
  return String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);
}

String longTimeStr(const time_t &t){
  // Retorna segundos como "d:hh:mm:ss"
  String s = String(t / SECS_PER_DAY) + ':';
  if (hour(t) < 10) {
    s += '0';
  }
  s += String(hour(t)) + ':';
  if (minute(t) < 10) {
    s += '0';
  }
  s += String(minute(t)) + ':';
  if (second(t) < 10) {
    s += '0';
  }
  s += String(second(t));
  return s;
}

String hexStr(const unsigned long &h, const byte &l = 8) {
  // Retorna valor em formato hexadecimal
  String s;
  s= String(h, HEX);
  s.toUpperCase();
  s = ("00000000" + s).substring(s.length() + 8 - l);
  return s;
}

String deviceID() {

    return "IeC-" + hexStr(ESP.getChipId());
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
void formatFS(){
  //Apaga todos os arquivos na região de memória reservada
  SPIFFS.format();
    Serial.println("Apagados todos os arquivos na região de memória reservada");
}

void deleteFile(void) {
  //Remove o arquivo
  if(SPIFFS.remove("/ConfigInit.json")){
    Serial.println("Arquivo removido com sucesso!");
  } else {
    Serial.println("Erro ao remover arquivo!");
  }
}

void openFS(){
  //Abre o sistema de arquivos
  if(!SPIFFS.begin()){
    Serial.println("Erro ao abrir o sistema de arquivos");
  } else {
    Serial.println("Sistema de arquivos aberto com sucesso!");
  }
}

void createFile(){
 
 char *arquivo[] = {"/ConfigInit.json", "/ConfigGeral.json"};
 
  for(int i = 0; i < 2; i++){  
  File wFile;
 
  //Cria o arquivo se ele não existir
    if(SPIFFS.exists(arquivo[i])){
    Serial.print("Arquivo \"");
    Serial.print(arquivo[i]);
    Serial.println("\" ja existe!");
    } else {
    Serial.println("Criando o arquivo...");
    wFile = SPIFFS.open(arquivo[i],"w+");
 
    //Verifica a criação do arquivo
    if(!wFile){
      Serial.print("Erro ao criar o arquivo ");
      Serial.print(arquivo[i]);
      Serial.println("!");
    } else {
      Serial.print("Arquivo ");
      Serial.print(arquivo[i]);
      Serial.println(" criado com sucesso!");
    }
  }
  wFile.close();
  }
}

boolean configRead() {
  // Lê configuração
  StaticJsonDocument<256> jsonConfigInit;

  File file = SPIFFS.open(F("/ConfigInit.json"), "r");
  if (deserializeJson(jsonConfigInit, file)) {
    // Falha na leitura, assume valores padrão
    configReset();//linha 247

    log(F("Falha lendo ConfigInit, assumindo valores padrão."));

    // Salva configuração
    configSave();   
    return false;
  } else {
      // Sucesso na leitura
     strlcpy(ssid, jsonConfigInit["ssid"]          | "", sizeof(ssid)); 
     strlcpy(pw, jsonConfigInit["pw"]              | "", sizeof(pw)); 

    file.close();

    Serial.println("\nLendo config:");
    serializeJsonPretty(jsonConfigInit, Serial);
    Serial.println("");

    return true;
  }
}

void  configReset() {
  // Define configuração padrão
 strlcpy(ssid, "", sizeof(ssid)); 
 strlcpy(pw, "", sizeof(pw)); 
  
}

boolean configSave() {
  // Grava configuração
   StaticJsonDocument<256> jsonConfigInit;

  File file = SPIFFS.open(F("/ConfigInit.json"), "w+");
  if (file) {
    // Atribui valores ao JSON e grava
    jsonConfigInit["ssid"]     = ssid;
    jsonConfigInit["pw"]       = pw;
  
    serializeJsonPretty(jsonConfigInit, file);
    file.close();

    log(F("\nGravando config:"));
    serializeJsonPretty(jsonConfigInit, Serial);
    log("");

    return true;
  }
  return false;
}

// Requisições Web --------------------------------------

void handleConfigInit() {
  // Config
  File file = SPIFFS.open(F("/ConfigInit.html"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    s.replace(F("#ssid#")      , ssid);

    // Send data
    server.send(200, F("text/html"), s);
    log("Config - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, F("text/plain"), F("Config - ERROR 500"));
    log(F("ConfigInit - ERRO lendo arquivo"));
  }
}

void handleConfigSave() {
  // Grava Config
  // Verifica número de campos recebidos
  // Gera o campo adicional "plain" via post
  if (server.args() == 3) {

    String s;
  
    // Grava ssid
    s = server.arg("ssid");
    s.trim();
    if( s != ""){
    strlcpy(ssid, s.c_str(), sizeof(ssid));
    }
    
    // Grava pw
    s = server.arg("pw");
    s.trim();
    if (s != "") {
      // Atualiza senha, se informada
      strlcpy(pw, s.c_str(), sizeof(pw));
    }

    // Grava configuração
    if (configSave()) {
      server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Configuração salva.');history.back()</script></html>"));
      log("ConfigSave - Cliente: " + ipStr(server.client().remoteIP()));
      ESP.reset();
    } else {
      server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Falha salvando configuração.');history.back()</script></html>"));
      log(F("ConfigSave - ERRO salvando Config"));
    }
  } else {
    server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Erro de parâmetros.');history.back()</script></html>"));
   
  }
}

void handleLogin() {
  // Login
  File file = SPIFFS.open(F("/Login.html"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Envia dados
    server.send(200, F("text/html"), s);
    log("Login - Cliente: " + ipStr(server.client().remoteIP()) +
        (server.uri() != "/" ? " [" + server.uri() + "]" : ""));
  } else {
    server.send(500, F("text/plain"), F("Login - ERROR 500"));
    log(F("Login - ERRO lendo arquivo"));
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
