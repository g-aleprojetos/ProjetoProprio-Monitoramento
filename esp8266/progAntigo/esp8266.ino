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
//definição de Ip fixo para NodeMCU
IPAddress ip(192,168,0,190); //número do ip a ser fixado
IPAddress gateway(192,168,0,1); //gateway de conexão
IPAddress subnet(255,255,255,0); //mascara de rede

WiFiServer server(80); //porta de internet a ser aberta
/************************************************************************************/
// --- Protótipo das Funções ---
void startOTA();
void ICACHE_RAM_ATTR onTimerISR();
long moving_average(int sig);
void temp();

/************************************************************************************/
// --- Constantes Auxiliares ---
#define     num    100         //número de iterações da média móvel

// --- Variáveis Globais ---
const char* ssid = STASSID;
const char* password = STAPSK;
int sensorAnalogico = A0;
int values[num];             //vetor com num posições, armazena os valores para cálculo da média móvel
int adc_noise1;              //armazena a leitura sem filtro da entrada analógica
int adc_noise2;              //armazena a leitura filtrada da entrada analógica
int offset = 40;
int valorTemp = 0;
float temperature = 0.0;       //Armazena a temperatura em graus Celsius
float current_temp = 0.0;      //Armazena a temperatura atual
char counter1  = 0x00;          //variável auxiliar de contagem
char counter2 = 0x00;
/************************************************************************************/

void setup() {
  Serial.begin(115200);                              //inicia a serial
  delay(10);                                         //intervalo 10 milisegundos
  
  Serial.println("Booting");                         //escreve o texto no serial
 // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);                        //passa os parâmetros para a função que vai fazer a conexão com o wifi
  WiFi.config(ip, gateway, subnet);                  //passa os parâmetros para a função que vai setar o ip fixo no NodeMCU

  while (WiFi.status() != WL_CONNECTED) {            //enquanto status for diferente de conectado
       delay(500);                                   //intervalo de 500 milisegundos
       Serial.print(".");                            //escreve o caracterer na serial
      }
  Serial.println("");                                //pula linha na janela do serial
  Serial.print("Conectado a rede sem fio: ");        //escreve o texto no serial
  Serial.println(ssid);                              //escreve o nome da rede wifi no serial
  Serial.println("Servidor iniciado");               //escreve o texto no serial
  server.begin(); //inicia o servidorpara receber dados na porta definida em "WiFiServer server(porta);"
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
      counter1++;                             // incrementa counter1
      counter2++;                             // incrementa counter1
  
    if(counter1 == 0x20)                      // counter 
      {                 // sim...
       digitalWrite(out, !digitalRead(out)); //Inverte o estado da saída de teste
        counter1 = 0x00;                      //zera o counter
       
       } //end if counter 

     if(counter2 == 0x05)                      // counter 
      {                                        // sim...
       temp();
        counter2 = 0x00;                       //zera o counter
       
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

 long moving_average(int sig)
{
  int i;               //variável auxiliar para iterações
  long acc = 0;        //acumulador
  
  //Desloca o vetor completamente eliminando o valor mais antigo
  for(i = num; i > 0; i--) values[i] = values[i-1];    
  
  values[0] = sig;           //carrega o sinal no primeiro elemento do vetor
  
   
  for(i = 0; i < num; i++) acc += values[i];
   
  
  return acc / num;         //Retorna a média móvel

} //end moving_average


void temp()
{
adc_noise1 = analogRead(sensorAnalogico);
adc_noise2 = moving_average(adc_noise1);
temperature = ((adc_noise2 * 3.3)/1024.0);
temperature = temperature * 100;
temperature = temperature - offset;

  if(temperature < (current_temp - 1.0) || temperature > (current_temp + 1.0))
  {
      current_temp = temperature;              //atualiza temperatura atual

       valorTemp = (int) temperature; 
     
      Serial.println(valorTemp);

   } //end if

  delay(80);
  
}//fim void temp
