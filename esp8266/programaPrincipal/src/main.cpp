#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define LED 2
#define out 16

// --- Configuração da rede e dispositivo ---
#ifndef STASSID
#define STASSID "g.aleprojetos"
#define STAPSK "a248314b"
#define DISPOSITIVO "device01"
#define SENHADISPOSITIVO "004545"
#endif

// --- Constantes Auxiliares ---

// --- Protótipo das Funções ---
void startOTA();

// --- Variáveis Globais ---
const char *ssid = STASSID;
const char *password = STAPSK;
bool auxConecao = true;
char counter = 0x00; //variável auxiliar de contagem
int x = 0;

// --- Rotina de Interrupção ---
void ICACHE_RAM_ATTR onTimerISR()
{

  timer1_write(100000);
  counter++; // incrementa counter

  if (counter == 0xff)                    // counter
  {                                       // sim...
    digitalWrite(out, !digitalRead(out)); //Inverte o estado da saída de teste
    Serial.println(x);
    x++;
    counter = 0x00; // zera o contador

  } //end if counter

} // fim da rotina de Interrupção

void setup()
{
  Serial.begin(9600); //inicia a serial
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    if (auxConecao)
    {
      Serial.println("Connecting.");
      auxConecao = false;
    }
    delay(500);
    Serial.print(".");
    //ESP.restart();
  }
  //Initialize Ticker every
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(100000);

  // --- Configuração do dispositivo ---

  startOTA();

  // configuração dos IOs
  pinMode(LED, OUTPUT);
  pinMode(out, OUTPUT);
}

void loop()
{
  ArduinoOTA.handle();

  digitalWrite(LED, HIGH);
  Serial.println("LED1");
  delay(100);
  digitalWrite(LED, LOW);
  Serial.println("LED1");
  delay(100);
}

// --- Desenvolvimento das Funções ---
void startOTA()
{
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(DISPOSITIVO); // Nome do dispositivo
  //ArduinoOTA.setPassword(SENHADISPOSITIVO); // Senha do dispositivo

  ArduinoOTA.onStart([]()
                     {
                       String type;
                       if (ArduinoOTA.getCommand() == U_FLASH)
                       {
                         type = "sketch";
                       }
                       else
                       { // U_SPIFFS
                         type = "filesystem";
                       }

                       // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                       Serial.println("Start updating " + type);
                     });

  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                       {
                         Serial.println("Auth Failed");
                       }
                       else if (error == OTA_BEGIN_ERROR)
                       {
                         Serial.println("Begin Failed");
                       }
                       else if (error == OTA_CONNECT_ERROR)
                       {
                         Serial.println("Connect Failed");
                       }
                       else if (error == OTA_RECEIVE_ERROR)
                       {
                         Serial.println("Receive Failed");
                       }
                       else if (error == OTA_END_ERROR)
                       {
                         Serial.println("End Failed");
                       }
                     });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
} //end startOTA