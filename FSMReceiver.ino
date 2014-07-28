#include <SD.h>
#include "State.h"

State *statePtr = 0;

unsigned int state = 0;
unsigned int ready = 0;

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    Serial.begin(115200);
    Serial.flush();
    digitalWrite(13, HIGH);
    if (!SD.begin(10))
    {
        Serial.println("initialization failed!");
    }
    else
    {
        ready = 1;
        digitalWrite(13, LOW);
        Serial.println("OK");
        delay(50);
        statePtr = new State();
    }
}

void loop()
{
    if (!ready)
    {
        Serial.println("HAVE A PROBLEM!!");
        delay(1000);
        return;
    }
    if (Serial.available() > 0)
    {
        // Serial.println(Serial.available());
        byte c = Serial.read();

        if (c == 0x1a)
        {
            if (state == 0)
            {
                state = 1;
            }
            else if (state == 1)
            {

                Serial.print("OK RESET.. <3 ");
                Serial.println(statePtr->get_state());


                if (statePtr->get_state() != 4)
                {
                    statePtr->reset(88);
                    delete statePtr;
                    statePtr = new State();
                }
                else
                {
                    statePtr->reset(99);
                }
                state = 0;
            }
        }
        else
        {
            if (state == 0)
            {
                statePtr->process(c);
            }
            else if (state == 1)
            {
                statePtr->process(0x1a);
                statePtr->process(c);
                state = 0;
            }

        }
    }

}


