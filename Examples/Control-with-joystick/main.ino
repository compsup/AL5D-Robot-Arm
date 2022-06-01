// Arduino pin numbers
const int left_SW_pin = 7;
const int left_x_pin = A0;
const int left_y_pin = A1;
const int right_SW_pin = 7;
const int right_x_pin = A5;
const int right_y_pin = A4;
int left_base_y, left_base_x = 0;
int right_base_y, right_base_x = 0;

#include <SoftwareSerial.h>

// Init software serial interface to SSC-32U
SoftwareSerial ssc32u(12, 13);

void setup()
{
    pinMode(left_SW_pin, INPUT);
    digitalWrite(left_SW_pin, HIGH);
    ssc32u.begin(9600);
    ssc32u.listen();
    Serial.begin(9600);
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
    left_base_y = analogRead(left_y_pin);
    left_base_x = analogRead(left_x_pin);
    right_base_y = analogRead(right_y_pin);
    right_base_x = analogRead(right_x_pin);
}
uint16_t pulse0 = 1500;
uint16_t pulse1 = 1500;
uint16_t pulse2 = 1500;
uint16_t pulse3 = 1500;
uint16_t pulse4 = 1500;
uint16_t pulse5 = 1500;

uint16_t pulses[] = {pulse0, pulse1, pulse2, pulse3, pulse4, pulse5};

void loop()
{
    // Formula to give us a proper step value too increase or decrease the pulse by.
    pulses[0] -= (analogRead(left_y_pin) - left_base_y) / 15;   // Base
    pulses[1] += (analogRead(left_x_pin) - left_base_x) / 25;   // Shoulder
    pulses[2] += (analogRead(right_y_pin) - right_base_y) / 25; // Elbow

    for (int i = 0; i < 6; i++)
    {
        if (pulses[i] >= 2500)
        {
            pulses[i] = 2500;
        }
        else if (pulses[i] <= 500)
        {
            pulses[i] = 500;
        }
    }

    // Motor instructions are as follows
    // #<motor_num>P<pulse_value>\r
    char output[80];
    sprintf(output, "#0P%u #1P%u #2P%u #3P%u #4P%u #5P%u\r", pulses[0], pulses[1], pulses[2], pulses[3], pulses[4], pulses[5]);
    ssc32u.write(output);
}