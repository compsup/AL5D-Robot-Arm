// Example code for a controller for the AL5D Robot Arm
// Created by Tyler Dakers and Duncan McQuat

// HARDWARE
// 2x Arduino Joystick Modules
// 1x Rotary Encoder Module
// 1x Breadboard (+5v and Ground channels, option joystick and rotary encoder pins)

// PINS
// Connect the RX pin on the SSC-32U to pin 13 and the TX pin to pin 12
// Connect +5v pins and GND pins to Arduino through Breadboard channels
// On left joystick connect VRX to pin A0, VRY to pin A1, and SW to pin 7
// On right joystick connect VRX to pin A5, VRY to pin A4, and SW to pin 8
// On rotary encoder connect CLK to pin 2, DT to pin 3, and SW to pin 4
#include <SoftwareSerial.h>

// Arduino pin numbers
const int left_SW_pin = 7;
const int left_x_pin = A0;
const int left_y_pin = A1;
const int right_SW_pin = 8;
const int right_x_pin = A5;
const int right_y_pin = A4;
const int CLK = 2;
const int DT = 3;
const int SW = 4;

//Encoder Setup
int currentStateCLK, lastStateCLK;
unsigned long lastButtonPress = 0;

// Offsets for joystick deadzone - get's setup at startup
int left_base_y, left_base_x;
int right_base_y, right_base_x;

// Init software serial interface to SSC-32U
SoftwareSerial ssc32u(12, 13);

void setup()
{
    // Pin layouts
    pinMode(left_SW_pin, INPUT);
    pinMode(right_SW_pin, INPUT);
    digitalWrite(left_SW_pin, HIGH);
    digitalWrite(right_SW_pin, HIGH);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    // Serial Setup
    Serial.begin(9600);
    Serial.println("===> System Init <===\n");
    Serial.print("Connecting too SSC-32U...");
    // ssc32u Controller Setup
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
    lastStateCLK = digitalRead(CLK);
    Serial.println(" Done\n");
    Serial.println("===> Starting Main Loop <===");

    // Call updateEncoder() when any high/low changed seen
    // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
    attachInterrupt(0, updateEncoder, CHANGE);
    attachInterrupt(1, updateEncoder, CHANGE);
}

// Define arrays for servo values, maxes, and minimums
uint16_t pulses[6] = {1500, 1500, 1500, 1500, 1500, 1500};
uint16_t max[6] = {2500, 2000, 1800, 2500, 2500, 2500};
uint16_t min[6] = {500, 800, 800, 500, 500, 500};

void loop()
{
    // Reset Servos to Base values when Motor-Encoder is pressed
    int btnState = digitalRead(SW);
    // If we detect LOW signal, button is pressed
    if (btnState == LOW)
    {
        // if 50ms have passed since last LOW pulse, it means that the
        // button has been pressed, released, and pressed again
        if (millis() - lastButtonPress > 50)
        {
            for (uint8_t i = 0; i < 6; i++)
            {
                pulses[i] = 1500;
            }
            // Make the reset motion smoother
            ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500 T1000 \r");
            delay(1000);
        }
        // Remember last button press event
        lastButtonPress = millis();
    }

    // open and close logic for gripper
    int left_btn_State = digitalRead(left_SW_pin);
    int right_btn_State = digitalRead(right_SW_pin);

    if (left_btn_State == 0)
    {
        pulses[4] += 40;
    } 
    else if (right_btn_State == 0)
    {   
        pulses[4] -= 40;
    }

    // Formula to give us a proper step value too increase or decrease the pulse by.
    pulses[0] -= (analogRead(left_y_pin) - left_base_y) / 25;   // Base
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
    // note: if time is at the end of the statement then it will effect all servos
    // #0P1500 #1P1500 #2P1500 T100
    char output[80];
    sprintf(output, "#0P%u #1P%u #2P%u #3P%uT100 #4P%u #5P%u\r", pulses[0], pulses[1], pulses[2], pulses[3], pulses[4], pulses[5]);
    ssc32u.write(output);
}

//Servo #5 Pulse Formula. Changed when the encoder is rotated
void updateEncoder()
{
    currentStateCLK = digitalRead(CLK);

    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {

        // If the DT state is different than the CLK state then
        // the encoder is rotating CCW so decrement
        if (digitalRead(DT) != currentStateCLK)
        {
            pulses[5] += 100;
        }
        else
        {
            pulses[5] -= 100;
        }
    }
    lastStateCLK = currentStateCLK;
}