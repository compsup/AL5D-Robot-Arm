#include <SoftwareSerial.h>

// Arduino pin numbers
const int left_SW_pin = 7;
const int left_x_pin = A0;
const int left_y_pin = A1;
const int right_SW_pin = 7;
const int right_x_pin = A5;
const int right_y_pin = A4;

// Offsets for joystick deadzone - get's setup at startup
int left_base_y, left_base_x;
int right_base_y, right_base_x;

// Init software serial interface to SSC-32U
SoftwareSerial ssc32u(12, 13);

void setup()
{
    pinMode(left_SW_pin, INPUT);
    digitalWrite(left_SW_pin, HIGH);

    Serial.begin(9600);
    Serial.println("===> System Init <===\n");

    Serial.print("Connecting too SSC-32U...");
    ssc32u.begin(9600);
    ssc32u.listen();
    Serial.println(" Done");

    // Center servos
    Serial.print("Centering Servos...");
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
    Serial.println(" Done");
    Serial.print("Calibrating Joysticks...");
    left_base_y = analogRead(left_y_pin);
    left_base_x = analogRead(left_x_pin);
    right_base_y = analogRead(right_y_pin);
    right_base_x = analogRead(right_x_pin);
    Serial.println(" Done\n");
    Serial.println("===> Starting Main Loop <===");
}

uint16_t pulses[6] = {1500, 1500, 1500, 1500, 1500, 1500};
uint16_t max[6] = {2500, 2500, 2500, 2500, 2500, 2500};
uint16_t min[6] = {500, 500, 500, 500, 500, 500};

void loop()
{
    // Formula to give us a proper step value too increase or decrease the pulse by.
    pulses[0] -= (analogRead(left_y_pin) - left_base_y) / 15;   // Base
    pulses[1] += (analogRead(left_x_pin) - left_base_x) / 25;   // Shoulder
    pulses[2] += (analogRead(right_x_pin) - right_base_x) / 25; // Elbow
    pulses[3] += (analogRead(right_y_pin) - right_base_y) / 25; // Wrist-vert

    // Prevent servos from over-extending
    for (uint8_t i = 0; i < 6; i++)
    {
        if (pulses[i] >= max[i])
        {
            pulses[i] = max[i];
        }
        else if (pulses[i] <= min[i])
        {
            pulses[i] = min[i];
        }
    }

    // Motor instructions are as follows, note: S and T are optional
    // #<servo_num>P<pulse>S<speed>T<time>\r     
    char output[80];
    sprintf(output, "#0P%u #1P%u #2P%u #3P%u #4P%u #5P%u\r", pulses[0], pulses[1], pulses[2], pulses[3], pulses[4], pulses[5]);
    ssc32u.write(output);
}