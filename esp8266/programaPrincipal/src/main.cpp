#include <Arduino.h>
#define LED 2
#define out 16

// --- Variáveis Globais ---
char counter = 0x00; //variável auxiliar de contagem

// --- Rotina de Interrupção ---
void ICACHE_RAM_ATTR onTimerISR(){

      timer1_write(100000);
      counter++;                              // incrementa counter
   
    if(counter == 0x05)                       // counter 
      {                                       // sim...
       digitalWrite(out, !digitalRead(out));  //Inverte o estado da saída de teste
        counter = 0x00;                       // zera o contador
       
       } //end if counter 
    
}// fim da rotina de Interrupção

void setup()
{
  Serial.begin(9600);//inicia a serial
  pinMode(LED, OUTPUT);
  pinMode(out, OUTPUT); 

    //Initialize Ticker every 
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(100000);  
 }

void loop()
{
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);
}