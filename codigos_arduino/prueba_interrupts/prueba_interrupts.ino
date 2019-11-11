/* FUENTE: https://www.luisllamas.es/que-son-y-como-usar-interrupciones-en-arduino/
 *  https://www.youtube.com/watch?v=up-goNfJ_EY&list=PLkjnQ3NFTPnY1eNyLDGi547gkVui1vyn2&index=21
 *  
 *  author: Luis Salgueiro
 *  date: 01/11/2019
 *  Description: Generate interruption through pin10
 *  
  */


int emuPin =10;

volatile int CONTADOR = 0;            // variable global CONTADOR
int ANTERIOR = 0 ;                   // variable para determinar si el valor de CONTADOR
                                    // ha cambiado desde la ultima vez
void setup(){
  Serial.begin(9600);       // inicializacion de comunicacion serie a 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(emuPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), sensor, RISING); // interrupcion sobre pin digital 2
}                                                           // LLAMA A LA FUNCION SENSOR CADA FLANCO ASCENDENTE
        
void loop(){
  
 //esta parte es para emular la salida digital que oscila entre 0-1
   digitalWrite(emuPin, HIGH);
   delay(1000);
   digitalWrite(emuPin, LOW);
   delay(1000);
 
  
  if (ANTERIOR != CONTADOR){      // si hubo un cambio respecto del valor anterior
    Serial.println(CONTADOR);     // imprime en monitor serial el valor de CONTADOR
    ANTERIOR = CONTADOR ;         // actualiza valor de ANTERIOR con nuevo valor
  }                               // de CONTADOR
}
   
void sensor(){        // rutina ISR
  CONTADOR++;       // incrementa valor de CONTADOR en 1
}
