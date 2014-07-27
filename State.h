#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include <SD.h>

// ----- Callback function types -----

extern "C" {
    typedef void (*callbackFunction)(void);
}

#define FILE_NAME_MAX_LENGTH 8
#define EXT_MAX_LEGNTH 3
#define FILE_TOTAL_LENGTH 14


class State
{
public:
    // ----- Constants -----
    static const int DefaultUnitTicks = 120; // slow enough for beginners

    // ----- Constructors -----
    State(void);

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
    bool is_file_name_ended(byte);
    bool reset(float);

private:
    int _unitTicks;

    // These variables that hold information across the upcomming tick calls.
    // They are declared tatic so the only get initialized once on program start and not
    // every time the tick function is called.
    int _state;
    unsigned long _startTime; // will be set for several states

    unsigned int _ready;
    unsigned int _tmp_name_counter;
    unsigned int _tmp_ext_counter;
    unsigned int _init_key_counter;

    char *_nextCode;
    char _file_name[FILE_TOTAL_LENGTH];

    File _myFile;
    int _chipSelect;

};

#endif



