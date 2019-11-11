/* led_builtin on-off with raspberry
 *  author=Luis Salgueiro
 *  date=03/11/2019
 *  sources: https://classes.engineering.wustl.edu/ese205/core/index.php?title=Serial_Communication_between_Raspberry_Pi_%26_Arduino
 *  https://forum.arduino.cc/index.php?topic=530649.0
 */

String c="";
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
  if (Serial.available()){
      c = Serial.readString();
      if(c=="ON"){
        digitalWrite(LED_BUILTIN,HIGH);
      }
      else if(c=="OFF"){
        digitalWrite(LED_BUILTIN,LOW);
      }
  
    delay(500);    
  }
}
