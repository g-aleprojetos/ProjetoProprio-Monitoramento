/*
 *  AUTOR: Alexandre Gonçalves
 *  EMAIL:g.aleprojetos@gmail.com
 *  ARQUIVO:dispositivo_esp8266.ino
 *  DATA:28/03/2021 upgrade 
 *  
 *  Informação do upgrade:
 *  Interrupção,
 *  ArduinoOta,
 *  Configuração de rede,
 *  fixar Ip,
 *  analógico
 */


// --- Bibliotecas Auxiliares ---

  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <DNSServer.h>
  #include <TimeLib.h>
  #include <FS.h>
  #include <ArduinoJson.h>
  
// --- Configuração da rede e dispositivo ---
const byte        WEBSERVER_PORT          = 80;                 // Porta Servidor Web
const char*       WEBSERVER_HEADER_KEYS[] = {"User-Agent"};     // Headers do Servidor Web
const byte        DNSSERVER_PORT          = 53;                 // Porta Servidor DNS
const byte        LED_PIN                 = LED_BUILTIN;        // ESP8266 possui pino padrão de LED
String            apWifi                  = "Wifi Automação";   // Nome do Wifi

//--- Estado do LED ---
const byte      LED_ON                  = LOW;
const byte      LED_OFF                 = HIGH;

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


// --- Protótipo das Funções ---


//--- Setup ---
void setup() {
     
  Serial.begin(9600);          // Velocidade para ESP8266

  if (!SPIFFS.begin()) {        // SPIFFS
    log(F("SPIFFS ERRO"));
    while (true);
  }
  
  configRead();                 // Lê configuração
  bootCount++;                  // Incrementa contador de inicializações
  configSave();                 // Salva configuração
  
  pinMode(LED_PIN, OUTPUT);     // LED
  ledSet();

  //--- WiFi Access Point ---
 
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP( apWifi );

     log("WiFi AP " + deviceID() + " - IP " + ipStr(WiFi.softAPIP()));

   //--- Habilita roteamento DNS ---
   dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
   dnsServer.start(DNSSERVER_PORT , "*", apIP);

  //--- WiFi Station ---
  WiFi.begin(ssid, pw);
  log("Conectando WiFi " + String(ssid));
  byte b = 0;
  while(WiFi.status() != WL_CONNECTED && b < 60) {
    b++;
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    // WiFi Station conectado
    log("WiFi conectado (" + String(WiFi.RSSI()) + ") IP " + ipStr(WiFi.localIP()));
  } else {
    log(F("WiFi não conectado"));
  }

  //--- WebServer ---
  server.on(F("/config")    , handleConfig);
  server.on(F("/configSave"), handleConfigSave);
  server.on(F("/reconfig")  , handleReconfig);
  server.on(F("/reboot")    , handleReboot);
  server.on(F("/css")       , handleCSS);
  server.onNotFound(handleHome);
  server.collectHeaders(WEBSERVER_HEADER_KEYS, 1);
  server.begin();

  //--- Pronto ---
  log(F("Pronto"));
}

// --- Loop --- 
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
  // Retorna ID padrão
 
    // ESP8266 utiliza função getChipId()
    return "ID interno - " + hexStr(ESP.getChipId()); 
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

void ledSet() {
  // Define estado do LED
  digitalWrite(LED_PIN, ledOn ? LED_ON : LED_OFF);
}

// Funções de Configuração ------------------------------
void  configReset() {
  // Define configuração padrão
 strlcpy(login, "admin", sizeof(login));
 strlcpy(senha, "admin", sizeof(senha));
 strlcpy(id, "Dispositivo", sizeof(id));
 strlcpy(ssid, "", sizeof(ssid)); 
 strlcpy(pw, "", sizeof(pw)); 
 strlcpy(masc, "", sizeof(masc)); 
 strlcpy(gat, "", sizeof(gat)); 
 bootCount = 0;
 ledOn     = false;
 strlcpy(idSensor,"sensor", sizeof(idSensor));
 sensor    = 25;  
  
}

boolean configRead() {
  // Lê configuração
  StaticJsonDocument<JSON_SIZE> jsonConfig;

  File file = SPIFFS.open(F("/Config.json"), "r");
  if (deserializeJson(jsonConfig, file)) {
    // Falha na leitura, assume valores padrão
    configReset();

    log(F("Falha lendo CONFIG, assumindo valores padrão."));
    return false;
  } else {
    // Sucesso na leitura
     strlcpy(login, jsonConfig["login"]        | "", sizeof(login));
     strlcpy(senha,  jsonConfig["senha"]       | "", sizeof(senha));
     strlcpy(id, jsonConfig["id"]              | "", sizeof(id)); 
     strlcpy(ssid, jsonConfig["ssid"]          | "", sizeof(ssid)); 
     strlcpy(pw, jsonConfig["pw"]              | "", sizeof(pw)); 
     strlcpy(masc, jsonConfig["masc"]          | "", sizeof(masc));
     strlcpy(gat,  jsonConfig["gat"]           | "", sizeof(gat));
     bootCount = jsonConfig["boot"]            | 0;
     ledOn     = jsonConfig["led"]             | false;
     strlcpy(idSensor, jsonConfig["idSensor"]  | "", sizeof(idSensor));
     sensor    = jsonConfig["sensor"]          | 25;

    file.close();

    log(F("\nLendo config:"));
    serializeJsonPretty(jsonConfig, Serial);
    log("");

    return true;
  }
}

boolean configSave() {
  // Grava configuração
  StaticJsonDocument<JSON_SIZE> jsonConfig;

  File file = SPIFFS.open(F("/Config.json"), "w+");
  if (file) {
    // Atribui valores ao JSON e grava
    jsonConfig["login"]    = login;
    jsonConfig["senha"]    = senha;
    jsonConfig["id"]       = id;
    jsonConfig["ssid"]     = ssid;
    jsonConfig["pw"]       = pw;
    jsonConfig["masc"]     = masc;
    jsonConfig["gat"]      = gat;
    jsonConfig["boot"]     = bootCount;
    jsonConfig["led"]      = ledOn;
    jsonConfig["idSensor"] = idSensor;
    jsonConfig["sensor"]   = sensor;


    serializeJsonPretty(jsonConfig, file);
    file.close();

    log(F("\nGravando config:"));
    serializeJsonPretty(jsonConfig, Serial);
    log("");

    return true;
  }
  return false;
}

// Requisições Web --------------------------------------
void handleHome() {
  // Home
  File file = SPIFFS.open(F("/Home.html"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    s.replace(F("#id#")       , id);
    s.replace(F("#serial#")   , hexStr(ESP.getChipId()));
    s.replace(F("#software#") , softwareStr());
    s.replace(F("#sysIP#")    , ipStr(WiFi.status() == WL_CONNECTED ? WiFi.localIP() : WiFi.softAPIP()));
    s.replace(F("#clientIP#") , ipStr(server.client().remoteIP()));
   /*****************************************************************/
    s.replace(F("#masc#")     , ipStr(server.client().remoteIP()));
    s.replace(F("#gat#")      , ipStr(server.client().remoteIP()));
/*********************************************************************/
    s.replace(F("#active#")   , longTimeStr(millis() / 1000));
    s.replace(F("#bootCount#"), String(bootCount));
    s.replace(F("#led#")      , ledOn ? F("Ligado") : F("Desligado"));
    s.replace(F("#idSensor#") , idSensor);
  //  s.replace(F("#temp#")      , sensor);
      
    // Envia dados
    server.send(200, F("text/html"), s);
    log("Home - Cliente: " + ipStr(server.client().remoteIP()) +
        (server.uri() != "/" ? " [" + server.uri() + "]" : ""));
  } else {
    server.send(500, F("text/plain"), F("Home - ERROR 500"));
    log(F("Home - ERRO lendo arquivo"));
  }
}

void handleConfig() {
  // Config
  File file = SPIFFS.open(F("/Config.htm"), "r");
  if (file) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    s.replace(F("#login#")     , login);
    s.replace(F("#senha#")     , senha);
    s.replace(F("#id#")        , id);
    s.replace(F("#ssid#")      , ssid);
    s.replace(F("#masc#")      , masc);
    s.replace(F("#gat#")       , gat);
    s.replace(F("#ledOn#")     ,  ledOn ? " checked" : "");
    s.replace(F("#ledOff#")    , !ledOn ? " checked" : "");
    // Send data
    server.send(200, F("text/html"), s);
    log("Config - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, F("text/plain"), F("Config - ERROR 500"));
    log(F("Config - ERRO lendo arquivo"));
  }
}

void handleConfigSave() {
  // Grava Config
  // Verifica número de campos recebidos

  // ESP8266 gera o campo adicional "plain" via post
  if (server.args() == 5) {

    String s;

   // Grava login
    s = server.arg("login");
    s.trim();
    strlcpy(login, s.c_str(), sizeof(login));

       // Grava senha
    s = server.arg("senha");
    s.trim();
    strlcpy(senha, s.c_str(), sizeof(senha));

    // Grava id
    s = server.arg("id");
    s.trim();
    if (s == "") {
      s = deviceID();
    }
    strlcpy(id, s.c_str(), sizeof(id));

    // Grava ssid
    s = server.arg("ssid");
    s.trim();
    strlcpy(ssid, s.c_str(), sizeof(ssid));

    // Grava pw
    s = server.arg("pw");
    s.trim();
    if (s != "") {
      // Atualiza senha, se informada
      strlcpy(pw, s.c_str(), sizeof(pw));
    }

       // Grava masc
    s = server.arg("masc");
    s.trim();
    strlcpy(masc, s.c_str(), sizeof(masc));

       // Grava gat
    s = server.arg("gat");
    s.trim();
    strlcpy(gat, s.c_str(), sizeof(gat));

    // Grava ledOn
    ledOn = server.arg("led").toInt();

    // Atualiza LED
    ledSet();

    // Grava configuração
    if (configSave()) {
      server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Configuração salva.');history.back()</script></html>"));
      log("ConfigSave - Cliente: " + ipStr(server.client().remoteIP()));
    } else {
      server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Falha salvando configuração.');history.back()</script></html>"));
      log(F("ConfigSave - ERRO salvando Config"));
    }
  } else {
    server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Erro de parâmetros.');history.back()</script></html>"));
  }
}

void handleReconfig() {
  // Reinicia Config
  configReset();

  // Atualiza LED
  ledSet();

  // Grava configuração
  if (configSave()) {
    server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Configuração reiniciada.');window.location = '/'</script></html>"));
    log("Reconfig - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(200, F("text/html"), F("<html><meta charset='UTF-8'><script>alert('Falha reiniciando configuração.');history.back()</script></html>"));
    log(F("Reconfig - ERRO reiniciando Config"));
  }
}

void handleReboot() {
  // Reboot
  File file = SPIFFS.open(F("/Reboot.htm"), "r");
  if (file) {
    server.streamFile(file, F("text/html"));
    file.close();
    log("Reboot - Cliente: " + ipStr(server.client().remoteIP()));
    delay(100);
    ESP.restart();
  } else {
    server.send(500, F("text/plain"), F("Reboot - ERROR 500"));
    log(F("Reboot - ERRO lendo arquivo"));
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
