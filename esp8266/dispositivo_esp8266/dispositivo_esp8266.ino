#define   out    D0                    // gpio02 led onbord vermelho

// --- Variáveis Globais ---
char counter  = 0x00;                  //variável auxiliar de contagem



// --- Rotina de Interrupção ---
void ICACHE_RAM_ATTR onTimerISR(){
  
      timer1_write(100000);
      counter++;               // incrementa counter
    
 
    if(counter == 0x05) // counter 
      {                 // sim...
       digitalWrite(out, !digitalRead(out)); //Inverte o estado da saída de teste
        counter = 0x00; 
       
       } //end if counter 
    
}
void setup() 
{
  Serial.begin(9600);                                          // Inicia a serial
  pinMode(out, OUTPUT);  
    
  //Initialize Ticker every 
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(100000); 
  
}

void loop() 
{

 
}
