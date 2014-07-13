#include "Nat.h"

// ----- Initialization -----

Nat::Nat(void)
{
  _unitTicks = DefaultUnitTicks;        // # millisec after single click is assumed.
  _tmp_counter = 0;
  _state = 0; // starting with state 0: waiting for a code to be sent.
} //Nat 

void Nat::tick(void) {
  unsigned long now = millis(); // current (relative) time in msecs.
  _startTime = now;
}
  
bool Nat::is_allowed(byte b) {
  Serial.println(b, DEC);
  if ((b >= 48 && b <= 57) || (b>=97 && b<= 122)) {
    Serial.println("true");
    return true;
  }
  else {
    Serial.println("false");
    return false;
  }
}

bool Nat::is_dot(byte d) {
  if (d == 0x2e) {
    return true;
  }
  else {
    return false;
  }
}

void Nat::process(byte b) {
  if (_state == 0) { // FIRST CHAR MUST BE CHAR!
    if (is_allowed(b)) {
      Serial.print("ALLOWED :");
      _file_name[0] = (char)b;
      Serial.print("CURRENT: ");
      Serial.println(String(_file_name));
      
      _state = 1;
      _tmp_counter++;
    }
    else {
      Serial.println("NOT ALLOWED");
    }
  }
  else if (_state == 1) { // NAME < 8 CHAR
     Serial.println("STATE 1");
     if (is_dot(b)) {
         _file_name[_tmp_counter] = b;
         _file_name[_tmp_counter+1] = '\0';
         Serial.println(String(_file_name));
       _state = 2;
       _tmp_counter = 0;
     }
     else {
       if (_tmp_counter <= NAME_MAX_LENGTH) {
         _file_name[_tmp_counter] = b;
         Serial.print("CONCAT: ");
         Serial.println(String(_file_name));         
         _tmp_counter++;
       }
       else {
         Serial.println("RESET STATE");
         _state = 0;
         //_tmp_counter = 0;
       }
     }
  }
  else if (_state == 2 ) {
    Serial.println("STATE 2 -- EXTENSION");
  }
}
