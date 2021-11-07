#include "ESP8266TimerInterrupt.h"

// --- Mapeamento de Hardware ---
#define BUILTIN_LED_STATUS            2       // Pin D4 on-board LED BLUE
#define BUILTIN_LED_ERROR            16       // Pin D0 on-board LED RED 

// --- Variáveis Globais ---
volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;
static bool started = false;
int counter1 = 0;
int counter2 = 0;

// --- Protótipo das Funções Auxiliares Externos ---
void configTime();

// --- Protótipo das Funções Auxiliares Internos ---


// --- Rotina de Interrupção ---
void IRAM_ATTR TimerHandler()
{
  counter1++;
  counter2++;
  
  if(counter1 >= 10){
    digitalWrite(BUILTIN_LED_STATUS,LOW); 
    if(counter1 == 11){
      digitalWrite(BUILTIN_LED_STATUS,HIGH); 
      counter1 = 0;
    }
  }
  
  if(counter2 == 15){
    digitalWrite(BUILTIN_LED_ERROR, !digitalRead(BUILTIN_LED_ERROR)); //Inverte o estado da saída de teste
    counter2 = 0;
  }
}

// --- Configurações Iniciais ---
void setup()
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED_STATUS, OUTPUT);
  pinMode(BUILTIN_LED_ERROR, OUTPUT);
  digitalWrite(BUILTIN_LED_STATUS,HIGH);
  digitalWrite(BUILTIN_LED_ERROR,HIGH);
  
  delay(200);
  configTime();

}

// --- Loop Infinito ---
void loop()
{
}

// --- Desenvolvimento das Funções Auxiliares Internos ---
