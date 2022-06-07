#include "al5d.h"
SoftwareSerial ssc32u(12, 13);

void al5d::begin()
{
    ssc32u.begin(9600);
    ssc32u.listen();
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
}
void al5d::set_motor(uint8_t motor_num, int degree, int time, int speed)
{
    int pulse = (11.1 * degree) + 1500;
    if (pulse < 500)
    {
        pulse = 500;
    }
    else if (pulse > 2500)
    {
        pulse = 2500;
    }
    motor_locs[motor_num] = degree;
    char output[80];
    sprintf(output, "#%uP%uS%uT%u\r", motor_num, pulse, speed, time);
    ssc32u.write(output);
}
void al5d::set_all_motors(int degree, int time, int speed)
{
    int pulse = (11.1 * degree) + 1500;
    if (pulse < 500)
    {
        pulse = 500;
    }
    else if (pulse > 2500)
    {
        pulse = 2500;
    }
    for (int i = 0; i < 6; i++)
    {
        motor_locs[i] = degree;
    }

    char output[80];
    sprintf(output, "#0P%uS%u #1P%uS%u #2P%uS%u #33P%uS%u #4P%uS%u #5P%uS%uT%u\r",
            pulse, speed,
            pulse, speed,
            pulse, speed,
            pulse, speed,
            pulse, speed,
            pulse, speed,
            time);
    ssc32u.write(output);
}
void al5d::center_motors()
{
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
}
int al5d::get_motor_loc(uint8_t motor_num)
{
    return motor_locs[motor_num];
}