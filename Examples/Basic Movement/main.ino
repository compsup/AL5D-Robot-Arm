// Example code for the AL5D Robot Arm
// Created by Tyler Dakers and Duncan McQuat

// PINS
// Connect the RX pin on the SSC-32U to pin 13 and the TX pin to pin 12
#include <SoftwareSerial.h>

// Init software serial interface to SSC-32U
SoftwareSerial ssc32u(12, 13);

void setup()
{
    // Init serial on USB port
    Serial.begin(9600);
    Serial.println("<==== System Initialising ====>");

    // Init software serial. Pins are (TX(12)/RX(13))
    Serial.println("");
    Serial.println("\tInit software serial interface :: ssc32u @9600...");
    ssc32u.begin(9600);
    ssc32u.listen();
    Serial.println("\tInit completed!");

    // Move motors to centered position in a group move
    Serial.println("\tCentering all motors...");
    Serial.println("\tMoving all motors to 1500 (#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\\r)");
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
    Serial.println("\tMoving motors completed!");
    Serial.println("");
    Serial.println("< Starting loop >");
    Serial.println("");
}

// Store value of current pulse
#define ARM_PULSE_STEP 8
#define ARM_PULSE_MAX 2000
#define ARM_PULSE_MIN 1000
uint16_t pulse = 1500;
uint8_t direction = 1;
uint16_t count = 0;

void loop()
{
    // Loop header
    count += 1;

    // Move arm position slightly
    Serial.print("\tArm currently ");
    Serial.println(pulse, DEC);
    if (direction)
    {
        // Increment pulse for each motor
        pulse += ARM_PULSE_STEP;

        // Check boundary
        if (pulse > ARM_PULSE_MAX)
        {
            pulse = ARM_PULSE_MAX;
            direction = 0;
        }
    }
    else
    {
        // Decrement pulse for each motor
        pulse -= ARM_PULSE_STEP;

        // Check boundary
        if (pulse < ARM_PULSE_MIN)
        {
            pulse = ARM_PULSE_MIN;
            direction = 1;
        }
    }
    Serial.print("\tArm moving to ");
    Serial.println(pulse, DEC);
    Serial.println("");

    // Motor instructions are as follows
    // #<motor_num>P<pulse_value>\r
    char output[80];
    sprintf(output, "#0P%u #1P%u #2P%u #3P%u #4P%u #5P%u\r", pulse, pulse, pulse, pulse, pulse, pulse);
    ssc32u.write(output);
}