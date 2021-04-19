// Bibliotecas ------------------------------------------
#ifdef ESP8266
  // Bibliotecas para ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <FS.h>
#else
  // Bibliotecas para ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <SPIFFS.h>
#endif
#include <DNSServer.h>
#include <TimeLib.h>
#include <ArduinoJson.h>

// Constantes -------------------------------------------
// Porta Servidor Web
const byte        WEBSERVER_PORT          = 80;

// Headers do Servidor Web
const char*       WEBSERVER_HEADER_KEYS[] = {"User-Agent"};

// Porta Servidor DNS
const byte        DNSSERVER_PORT          = 53;

// Pino do LED
#ifdef ESP32
  // ESP32 não possui pino padrão de LED
  const byte      LED_PIN                 = 2;
#else
  // ESP8266 possui pino padrão de LED
  const byte      LED_PIN                 = LED_BUILTIN;
#endif

// Estado do LED
#ifdef ESP8266
  // ESP8266 utiliza o estado inverso
  const byte      LED_ON                  = LOW;
  const byte      LED_OFF                 = HIGH;
#else
  // ESP32 utilizam estado normal
  const byte      LED_ON                  = HIGH;
  const byte      LED_OFF                 = LOW;
#endif

// Tamanho do Objeto JSON
const   size_t    JSON_SIZE               = JSON_OBJECT_SIZE(5) + 130;

// Instâncias -------------------------------------------
// Web Server
#ifdef ESP8266
  // Classe WebServer para ESP8266
  ESP8266WebServer  server(WEBSERVER_PORT);
#else
  // Classe WebServer para ESP32
  WebServer  server(WEBSERVER_PORT);
#endif

// DNS Server
DNSServer         dnsServer;

// Variáveis Globais ------------------------------------
char              login[30];     // login
char              senha[30];     // senha

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

void ledSet() {
  // Define estado do LED
  digitalWrite(LED_PIN, ledOn ? LED_ON : LED_OFF);
}

// Setup -------------------------------------------
void setup() {

}

// Loop --------------------------------------------
void loop() {

}
