#include <al5d.h>

const int input = 8;
int pulse = 0;
int var = 0;

int timer = 0;

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
int8_t speed = 100;

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

  if (pulse == 8 && timer == 50) //7 Dialed
  {
    speed += 
  }
    else if (pulse == 9 && timer == 50) //8 Dialed
  {
    num = 4;
    degree = -75;
  }
    else if (pulse == 10 && timer == 50) //9 Dialed
  {
    num = 4;
    degree = -75;
  }
    else if (pulse == 11 && timer == 50) //0 Dialed
  {
    num = 4;
    degree = -75;
  }

  move_motor(var);
 
  AL5D.set_motor(num, degree, speed);

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

  delay(5);
}

void move_motor(uint8_t num)
{
  num --;

    if (pulse == 2 && timer == 50) //1 Dialed
    {
      degree = 5;
    }
    else if (pulse == 3 && timer == 50) //2 Dialed
    {
      degree = -5;
    }
    else if (pulse == 4 && timer == 50) //3 Dialed
    {
      degree = 10;
    }
    else if (pulse == 5 && timer == 50) //4 Dialed
    {
      degree = -10;
    }
    else if (pulse == 6 && timer == 50) //5 Dialed
    {
      degree = 20;
    }
    else if (pulse == 7 && timer == 50) //6 Dialed
    {
      degree = -20;
    }


}