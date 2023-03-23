#include "al5d.h"

al5d AL5D;

void setup()
{
    AL5D.begin();
}

int8_t degree = 20;

void loop()
{
    if (AL5D.get_motor_loc(0) == 20)
    {
        degree = -20;
    }
    else if (AL5D.get_motor_loc(0) == -20)
    {
        degree = 20;
    }
    AL5D.set_all_motors(degree, 2000);
    delay(2000);
}