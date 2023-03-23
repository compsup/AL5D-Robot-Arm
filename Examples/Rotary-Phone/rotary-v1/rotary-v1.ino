#include <al5d.h>

#include <al5d.h>

const int input = 8;
int pulse = 0;
int var = 0;

int timer = 0;
int A = 0;
int B = 0;

al5d AL5D;

void setup()
{
  pinMode(input,INPUT);
  Serial.begin(9600);
  Serial.println(F("No pulses yet..."));

  // Robotic Arm Startup
  AL5D.begin();
}

int8_t degree;
uint8_t num;
int8_t time = 5;

void loop() 
{
  
  timer++;

  // Pulse Detected
  if(digitalRead(input)>var)
  {
    var = 1;
    pulse++;
    timer = 0;
  }
  if (pulse > 1 && timer == 101 )
  {
  if (pulse == 2) //1 Dialed
  {
    num = 4;
    degree = -75;
  }
  else if (pulse == 3 ) //2 Dialed
  {
    num = 4;
    degree = 75;
  }
    else if (pulse == 4 ) //3 Dialed
  {
    num = 3;
    degree = AL5D.get_motor_loc(num) - 20;
  }
  
    else if (pulse == 5 ) //4 Dialed
  {
    num = 3;
    degree = AL5D.get_motor_loc(num) + 20;
  }
  
    else if (pulse == 6) //5 Dialed
  {
    num = 2;
    degree = AL5D.get_motor_loc(num) + 20;
  }
  
    else if (pulse == 7) //6 Dialed
  {
    num = 2;
    degree = AL5D.get_motor_loc(num) - 20;
  }
  
    else if (pulse == 8) //7 Dialed
  {
    num = 1;
    degree = AL5D.get_motor_loc(num) - 20;
  }
  
    else if (pulse == 9) //8 Dialed
  {
    num = 1;
    degree = AL5D.get_motor_loc(num) + 20;
  }
  
    else if (pulse == 10) //9 Dialed
  {
    num = 0;
    degree = AL5D.get_motor_loc(num) - 20;
  }
    else if (pulse == 11) //0 Dialed
  {
    num = 0;
    degree = AL5D.get_motor_loc(num) + 20;
  }
  }
  AL5D.set_motor(num, degree, 300);

  // Write Determined Pulse Value
    if(pulse > 1 && timer == 50)
  {
    Serial.print(pulse - 1);
    pulse = 1;
  }

  // Reset Variable Count when no pulse detected
  if(digitalRead(input) == 0)
  {
    var=0;
  }

  // Reset Timer after 5 seconds
  if(timer==1000)
  {
    pulse = 1;
    timer = 0;
  }

  
}
