#include <SD.h>
#include "State.h"

State *nat = 0;

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
        nat = new State();
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
                Serial.println(nat->get_state());


                if (nat->get_state() != 4)
                {
                    nat->reset(88);
                    delete nat;
                    nat = new State();
                }
                else
                {
                    nat->reset(99);
                }
                state = 0;
            }
        }
        else
        {
            if (state == 0)
            {
                nat->process(c);
            }
            else if (state == 1)
            {
                nat->process(0x1b);
                nat->process(c);
                state = 0;
            }

        }
    }

}


