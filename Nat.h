#ifndef NAT_H 
#define NAT_H 

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}

#define FILE_NAME_MAX_LENGTH 8
#define FILE_TOTAL_LENGTH 11


class Nat 
{
public:
  // ----- Constants -----
  static const int DefaultUnitTicks = 120; // slow enough for beginners

  // ----- Constructors -----
  Nat(void);

  // ----- Runtime Parameters -----

  // set # millisec for one morse unit.
  void setUnitTicks(int ticks);

  // ----- State machine functions -----

  // start sending this message like ".- .-. -.. ..- .. -. ---"
  void sendCode(char *msg);

  // call this function every some milliseconds.
  void tick(void);
  
  void process(byte);
  
  bool is_allowed(byte);
  bool is_dot(byte);

private:
  int _unitTicks;

  // These variables that hold information across the upcomming tick calls.
  // They are declared tatic so the only get initialized once on program start and not
  // every time the tick function is called.
  int _state;
  unsigned long _startTime; // will be set for several states
  
  unsigned int _tmp_counter;

  char *_nextCode;
  char _file_name[12];
};

#endif



