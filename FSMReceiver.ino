#include <SD.h>
#include "State.h"

State *nat = 0;

byte prevChar = 0x0;

unsigned int ready = 0;

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    Serial.begin(9600);
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
        delay(200);
        nat = new State();
    }


}

void loop()
{
    if (!ready)
    {
        Serial.println("NOT READY");
        delay(1000);
        return;
    }

    static int n = 0;
    nat->tick();


    if (Serial.available() > 0)
    {

        byte c = Serial.read();

        if (prevChar == 0x1a)
        {
            if (c == 0x1A)
            {
                Serial.println("RESET...");
            }
            else
            {
                Serial.println("___..");
                if (prevChar == 0x0)
                {
                    Serial.println("----PROCESS ONLY C");
                    nat->process(c);
                }
                else
                {
                    Serial.println("----PROCESS TWO CHARS");
                    nat->process(prevChar);
                    nat->process(c);
                }
            }
        }
        else
        {
            nat->process(c);
            // Serial.println("++PROCESS..");
        }
        prevChar = c;
    }

// keep watching the push button:

    delay(1);
}


