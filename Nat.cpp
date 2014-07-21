#include "Nat.h"

// ----- Initialization -----

Nat::Nat(void)
{
    _unitTicks = DefaultUnitTicks;        // # millisec after single click is assumed.
    _tmp_name_counter = 0;
    _tmp_ext_counter = 0;
    _state = 0; // starting with state 0: waiting for a code to be sent.
} //Nat

void Nat::tick(void)
{
    unsigned long now = millis(); // current (relative) time in msecs.
    // _startTime = now;
    if (now - _startTime >= 1000) {
        _startTime = now;
        Serial.print("TICKING... ");
        Serial.println(_startTime);
    }
}

bool Nat::reset()
{
    for (int i = 0; i < FILE_TOTAL_LENGTH; ++i)
    {
        _file_name[i] = '\0';
    }
}

bool Nat::is_allowed(byte b)
{
    if ((b >= 48 && b <= 57) || (b>=97 && b<= 122))
    {
        // Serial.println("true");
        return true;
    }
    else
    {
        // Serial.println("false");
        return false;
    }
}

bool Nat::is_dot(byte d)
{
    if (d == 0x2e)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Nat::is_file_name_ended(byte b)
{
    // 0x3b = ;
    if (b == 0x3b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Nat::process(byte b)
{
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
            Serial.println("NOT ALLOWED: @0");
            reset();
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
                reset();
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
            Serial.print("== ");
            Serial.print(pos);
            Serial.print(", ");
            Serial.println(_tmp_ext_counter);

            if (_tmp_ext_counter > EXT_MAX_LEGNTH-1)
            {
                reset();
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
        }
        else
        {
            Serial.println("RESET STATE @2.2");
            _state = 0;
            reset();
        }
    }
    else if (_state == 3)
    {
        Serial.println(".. STATE 3 ..");
        _myFile = SD.open(_file_name, FILE_WRITE);

        // if the file opened okay, write to it:
        if (_myFile)
        {
            _state = 4;
        }
        else 
        {
            Serial.print("error opening ");
            Serial.println(_file_name);
            reset();
            _state = 0;
        }
    }
    else if (_state == 4) {
        Serial.println(".. STATE 4 ..");
        if (is_file_name_ended(b)) {
            Serial.flush();
            _myFile.close();
        }
        else {
            _myFile.write(b);
        }
    }


    Serial.println();
    Serial.println("=========");
    Serial.print("STATE.. ");
    Serial.println(_state);
    Serial.print("FILE: ");
    Serial.println(_file_name);;
    Serial.println("=========");
}
