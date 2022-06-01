// Arduino pin numbers
const int left_SW_pin = 7; // digital pin connected to switch output
const int left_X_pin = A0; // analog pin connected to X output
const int left_Y_pin = A1; // analog pin connected to Y output
int left_base_x = 0;

#include <SoftwareSerial.h>

// Init software serial interface to SSC-32U
SoftwareSerial ssc32u(12, 13);

void setup() {
    ssc32u.listen();
    pinMode(left_SW_pin, INPUT);
    digitalWrite(left_SW_pin, HIGH);
    Serial.begin(9600);
    ssc32u.write("#0P1500 #1P1500 #2P1500 #3P1500 #4P1500 #5P1500 #6P1500\r");
    left_base_x = analogRead(left_X_pin);
}
int8_t step0 = 0;
uint16_t pulse = 1500;
uint16_t pulse1 = 1500;
uint16_t pulse2 = 1500;
uint16_t pulse3 = 1500;
uint16_t pulse4 = 1500;
uint16_t pulse5 = 1500;

void loop() {
    
    step0 = (analogRead(left_X_pin) - left_base_x) * 0.02;
    pulse += step0;

    Serial.print("Pulse: ");
    Serial.println(pulse);
    Serial.print("Step: ");
    Serial.println(step0);
    Serial.print("left_base_x: ");
    Serial.println(left_base_x);
    Serial.print("X_val: ");
    Serial.println(analogRead(left_X_pin));
    // Motor instructions are as follows
    // #<motor_num>P<pulse_value>\r
    char output[80];
    sprintf(output, "#0P%u #1P%u #2P%u #3P%u #4P%u #5P%u\r", pulse, pulse1, pulse2, pulse3, pulse4, pulse5);
    ssc32u.write(output);
}