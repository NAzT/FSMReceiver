#include "Nat.h"

Nat nat;

void setup() 
{
  Serial.begin(9600);

}

void loop() 
{
  static int n = 0;
  nat.tick(); 
  
  if (Serial.available() > 0) {
    Serial.print("ENTER: ");
    char c = Serial.read();
    Serial.println(c);
    nat.process(c);
  }
  
  // keep watching the push button:

  delay(1);
}


