#include <SD.h>
#include "State.h"

State *nat = 0;

void setup()
{
    pinMode(10, OUTPUT);
    Serial.begin(9600);
    Serial.flush();

    if (!SD.begin(10))
    {
        Serial.println("initialization failed!");
    }
    else
    {
        Serial.println("OK");
        delay(200);
        nat = new State();
    }


}

void loop()
{
    static int n = 0;
    nat->tick();

    if (Serial.available() > 0)
    {
        char c = Serial.read();
        nat->process(c);
    }

    // keep watching the push button:

    delay(1);
}


