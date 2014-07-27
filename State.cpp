#include "State.h"

// ----- Initialization -----
#define DEBUG_VERBOSE 0
#define DEBUG 1

State::State(void)
{
    _init_key_counter = 0;
    _unitTicks = DefaultUnitTicks;        // # millisec after single click is assumed.
    _tmp_name_counter = 0;
    _tmp_ext_counter = 0;
    _state = 0; // starting with state 0: waiting for a code to be sent.
} //State


int State::get_state() {
    return _state;
}



void State::tick(void)
{
    unsigned long now = millis(); // current (relative) time in msecs.
    // _startTime = now;
    if (now - _startTime >= 1000)
    {
        _startTime = now;
    }
}

void State::reset(float state)
{
    Serial.print("LOCAL STATE__ ");
    Serial.println(_state);

    if (_state == 4)
    {
        Serial.println("===DONE==");

        Serial.flush();
        _myFile.close();
        _state = 0;
    }
    else 
    {
        Serial.print("RESET AT: ");
        Serial.println(state);
    }
    //
    for (int i = 0; i < FILE_TOTAL_LENGTH; ++i)
    {
        _file_name[i] = '\0';
    }
}

bool State::is_allowed(byte b)
{
    bool allowed = (b >= 48 && b <= 57) || (b>=97 && b<= 122);
    return allowed;
}

bool State::is_dot(byte d)
{
    return d == 0x2e;
}

bool State::is_file_name_ended(byte b)
{
    // 0x3b = ;
    return b == 0x3b;
}

void State::process(byte b)
{

    Serial.print(b, HEX);
    if (_state == 0)   // FIRST CHAR MUST BE CHAR!
    {
        if (is_allowed(b))
        {
            _state = 1;
            _file_name[0] = (byte)b;
            _file_name[1] = '\0';
            _tmp_name_counter++;
        }
        else
        {
            if (DEBUG_VERBOSE)
            {
                Serial.print(b, HEX);
                Serial.println(" NOT ALLOWED: @0");
            }
            reset(0.5);
            _state = 0;
        }
    }
    else if (_state == 1)   // NAME < 8 CHAR
    {
        if (is_dot(b))
        {
            _file_name[_tmp_name_counter++] = b;
            _state = 2;
        }
        else
        {
            if (_tmp_name_counter < FILE_NAME_MAX_LENGTH)
            {
                _file_name[_tmp_name_counter] = b;
                _tmp_name_counter++;
            }
            else
            {
                reset(1);
                _tmp_name_counter = 0;
                _state = 0;
            }
        }
    }
    else if (_state == 2 )   //GRAB EXTENSION
    {
        if (is_allowed(b))
        {
            unsigned pos = _tmp_name_counter + _tmp_ext_counter;
            if (DEBUG_VERBOSE)
            {
                Serial.print("== ");
                Serial.print(pos);
                Serial.print(", ");
                Serial.println(_tmp_ext_counter);
            }

            if (_tmp_ext_counter > EXT_MAX_LEGNTH-1)
            {
                reset(2.1);
                _state = 0;
                _tmp_name_counter = 0;
                _tmp_ext_counter = 0;
            }
            else
            {
                _file_name[pos] = b;
                _tmp_ext_counter++;
            }
        }
        else if (is_file_name_ended(b))
        {
            unsigned int len = strlen(_file_name);
            _file_name[len] = '\0';
            _state = 3;
            Serial.println();
            Serial.print("OK GOT:");
            Serial.println(_file_name);
        }
        else
        {
            if (DEBUG_VERBOSE)
            {
                Serial.println();
                Serial.println("RESET STATE @2.2");
            }
            _state = 0;
            reset(2.2);
        }
    }
    else if (_state == 3)
    {
        if (DEBUG_VERBOSE)
        {
            Serial.println();
            Serial.println(".. STATE 3 ..");
        }


        if (SD.exists(_file_name)) {
            SD.remove(_file_name);
        }

        _myFile = SD.open(_file_name, FILE_WRITE);

        // if the file opened okay, write to it:
        if (_myFile)
        {
            _myFile.write(b);
            _state = 4;
            Serial.println();
            Serial.println(".. STATE 4 ..");
        }
        else
        {
            if (DEBUG_VERBOSE)
            {
                Serial.println();
                Serial.print("error opening ");
                Serial.println(_file_name);
            }
            reset(3);
            _state = 0;
        }
    }
    else if (_state == 4)
    {

        if (is_file_name_ended(b))
        {
            // Serial.println("DONE");
            // Serial.flush();
            // _myFile.close();
            // _state = 0;
            // reset(4);
        }
        else
        {
            _myFile.write(b);
        }
    }

    if (DEBUG_VERBOSE)
    {
        Serial.println();
        Serial.println("=========");
        Serial.print("STATE.. ");
        Serial.println(_state);
        Serial.print("FILE: ");
        Serial.println(_file_name);;
        Serial.println("=========");
    }

}
