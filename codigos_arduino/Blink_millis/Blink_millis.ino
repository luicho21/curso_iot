/*
  Blink with millis
  author: Luis Salgueiro
  date: 01/11/2019

*/

int estado = LOW;
int delay_ms = 3000;
int millis_init = 0;
//int pin_LED = 6;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(100);     // wait for a second

// DEBUGING
//  estado =!estado;
//  Serial.println(millis());  // PRINTING MILLIS VALUE EVERY 1 SEC
//  Serial.println(estado);  // PRINTING estado VALUE EVERY 1 SEC
//  delay(1000);

  unsigned long millis_actual = millis();
  if ((millis_actual - millis_init) > delay_ms){
    millis_init = millis_actual;
    estado = !estado;
    digitalWrite(LED_BUILTIN, estado);
  }

  
  


}
