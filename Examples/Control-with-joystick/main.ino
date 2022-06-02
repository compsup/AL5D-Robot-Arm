#include <SoftwareSerial.h>

// Arduino pin numbers
const int left_SW_pin = 7;
const int left_x_pin = A0;
const int left_y_pin = A1;
const int right_SW_pin = 7;
const int right_x_pin = A5;
const int right_y_pin = A4;
#define CLK 2
#define DT 3
#define SW 4

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

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

    // Set encoder pins as inputs
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    // Read the initial state of CLK
    lastStateCLK = digitalRead(CLK);

    // Center servos
    Serial.print("Centering Servos...");
    ssc32u.write("#0P0 #1P0 #2P0 #3P0 #4P0 #5P0 #6P0\r");
    Serial.println(" Done");
    Serial.print("Calibrating Joysticks...");
    left_base_y = analogRead(left_y_pin);
    left_base_x = analogRead(left_x_pin);
    right_base_y = analogRead(right_y_pin);
    right_base_x = analogRead(right_x_pin);
    Serial.println(" Done\n");
    Serial.println("===> Starting Main Loop <===");

    // Call updateEncoder() when any high/low changed seen
    // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
    attachInterrupt(0, updateEncoder, CHANGE);
    attachInterrupt(1, updateEncoder, CHANGE);
}

uint16_t pulses[6] = {1500, 1500, 1500, 1500, 1500, 1500};
uint16_t max[6] = {2500, 2500, 2500, 2500, 2500, 2500};
uint16_t min[6] = {500, 500, 500, 500, 500, 500};

void loop()
{
    // Read the button state
    int btnState = digitalRead(SW);

    // If we detect LOW signal, button is pressed
    if (btnState == LOW)
    {
        // if 50ms have passed since last LOW pulse, it means that the
        // button has been pressed, released and pressed again
        if (millis() - lastButtonPress > 50)
        {
            for (uint8_t i = 0; i < 6; i++)
            {
                pulses[i] = 1500;
            }
        }

        // Remember last button press event
        lastButtonPress = millis();
    }
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
    sprintf(output, "#0P%uT100 #1P%uT100 #2P%uT100 #3P%uT100 #4P%u #5P%u\r", pulses[0], pulses[1], pulses[2], pulses[3], pulses[4], pulses[5]);
    ssc32u.write(output);
}

void updateEncoder()
{
    // Read the current state of CLK
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
            // Encoder is rotating CW so increment
            pulses[5] -= 100;
        }

        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }

    // Remember last CLK state
    lastStateCLK = currentStateCLK;
}