#ifndef AL5D_H
#define AL5D_H
#include <Arduino.h>
#include <SoftwareSerial.h>
class al5d
{
public:
    void begin();
    void set_motor(uint8_t motor_num, int degree, int time = 500, int speed = 750);
    void set_all_motors(int degree, int time = 500, int speed = 750);
    void center_motors();
};
#endif