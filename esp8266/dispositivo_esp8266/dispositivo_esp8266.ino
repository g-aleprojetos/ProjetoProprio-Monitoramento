/*
 *  AUTOR: Alexandre Gonçalves
 *  EMAIL:g.aleprojetos@gmail.com
 *  ARQUIVO:dispositivo_esp8266.ino
 *  DATA:28/03/2021 upgrade 
 *  
 *  Informação do upgrade:
 *  Interrupção,
 *  ArduinoOta,
 *  Configuração de rede
 */

// --- Bibliotecas Auxiliares ---
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
/************************************************************************************/
// --- Configuração da rede e dispositivo ---
#ifndef STASSID
#define STASSID "g.aleprojetos"        //configura o nome da rede wifi
#define STAPSK  "a248314b"             //configura a senha da rede wifi
#define DISPOSITIVO "dispositivo 01"   //configura o nome do dispositivo no OTA
#define SENHADISPOSITIVO "004545"      //configura a senha do despositivo no OTA
#define   out    D0                    // gpio02 led onbord vermelho
#endif

/************************************************************************************/
// --- Protótipo das Funções ---
void startOTA();
void ICACHE_RAM_ATTR onTimerISR();

/************************************************************************************/
// --- Variáveis Globais ---
const char* ssid = STASSID;
const char* password = STAPSK;
char counter  = 0x00;                  //variável auxiliar de contagem

/************************************************************************************/

void setup() {
  Serial.begin(115200);                              //inicia a serial
  delay(10);                                         //intervalo 10 milisegundos
  
  Serial.println("Booting");                         //escreve o texto no serial
 // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);                        //passa os parâmetros para a função que vai fazer a conexão com o wifi

  while (WiFi.status() != WL_CONNECTED) {            //enquanto status for diferente de conectado
       delay(500);                                   //intervalo de 500 milisegundos
       Serial.print(".");                            //escreve o caracterer na serial
      }
  Serial.println("");                                //pula linha na janela do serial
  Serial.print("Conectado a rede sem fio: ");        //escreve o texto no serial
  Serial.println(ssid);                              //escreve o nome da rede wifi no serial
  Serial.println("Servidor iniciado");               //escreve o texto no serial

  Serial.print("IP para se conectar ao NodeMCU: ");  //escreve o texto no serial
  Serial.print("http://");                           //escreve o texto no serial
  Serial.println(WiFi.localIP());                    //escreve na serial o IP recebido dentro da rede wifi (o ip nesse caso é recebido automaticamente)
 
 // --- Configuração do dispositivo ---
  pinMode(out, OUTPUT); 
 
  //Initialize Ticker every 
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(100000); 

  startOTA();
  
}

void loop() {
  ArduinoOTA.handle();
}

// --- Desenvolvimento das Funções ---
// --- Rotina de Interrupção ---
void ICACHE_RAM_ATTR onTimerISR(){
  
      timer1_write(100000);
      counter++;                             // incrementa counter
  
    if(counter == 0x20)                      // counter 
      {                 // sim...
       digitalWrite(out, !digitalRead(out)); //Inverte o estado da saída de teste
        counter = 0x00;                      //zera o counter
       
       } //end if counter 
}

// --- Start OTA ---
void startOTA(){
  ArduinoOTA.setHostname(DISPOSITIVO);        // Nome do dispositivo
  ArduinoOTA.setPassword(SENHADISPOSITIVO);   // Senha do dispositivo
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 }//end startOTA
