#include <SD.h>
#include "State.h"

State *state = 0;

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
        state = new State();
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

        if (c == 0x1b)
        {
            if (state == 0)
            {
                state = 1;
            }
            else if (state == 1)
            {

                Serial.print("OK RESET.. <3 ");
                Serial.println(state->get_state());


                if (state->get_state() != 4)
                {
                    state->reset(88);
                    delete state;
                    state = new State();
                }
                else
                {
                    state->reset(99);
                }
                state = 0;
            }
        }
        else
        {
            if (state == 0)
            {
                state->process(c);
            }
            else if (state == 1)
            {
                state->process(0x1b);
                state->process(c);
                state = 0;
            }

        }
    }

}


