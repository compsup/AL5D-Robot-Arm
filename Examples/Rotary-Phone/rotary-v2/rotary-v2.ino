// This program utilizes a rotary phone to execute functions on the AL5D robotic arm.
// The instructions are as follows:

/* :
1 = Select Base (1)
2 = Select Shoulder (2)
3 = Select Elbow (3)
4 = Select Vertical Wrist (4)
5 = Select Grip (5)
6 = Select Rotate Wrist (6)
7 = Increase Motor Speed
8 = Decrease Motor Speed
9 = Reset all Motors
*/

/* When Motor Selection occurs, a new set of commands is availible to increment the degree of the servos:
1 = Increase by 5
2 = Decrease by 5
3 = Increase by 10
4 = Decrease by 10
5 = Increase by 20
6 = Decrease by 20
7/8 = Increment Speed
9 = Reset Current Motor
10 = Deselect Motor
* once a command has been executed, another command can be inputed. To return to initial selection, delect current motor
*/

#include <al5d.h>
#include <LiquidCrystal_I2C.h>

const int input = 8;
uint8_t pulse = 0;
int var = 0;
int timer = 0;

int8_t degree;
int8_t increment;
uint8_t num;
float time = 500;
int toggle = 0;

al5d AL5D;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  pinMode(input,INPUT);
  Serial.begin(9600);
  // Robotic Arm Startup
  AL5D.begin();

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.print("AL5D Robotic Arm");
  lcd.setCursor(1, 1);
  lcd.print("w Rotory Phone");
  delay(3000);
  DefaultMenu();
}


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

  if (pulse > 1 && timer == 10 )
  {
    if (pulse == 8) //7 Dialed
    {
      if ( time > 0 )
      {
        time -= 500;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Speed: ");
      lcd.print(time/1000);
      lcd.print("s");
      delay(1000);
      DefaultMenu();
    }

    else if (pulse == 9) //8 Dialed
    {
      time += 500;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Speed: ");
      lcd.print(time/1000);
      lcd.print("s");
      delay(1000);
      DefaultMenu();
    }

    else if (toggle == 1) //Dials will move motors x degrees
    {
      if (pulse == 2) //1 Dialed
      {
        increment = 5;
      }
      else if (pulse == 3 ) //2 Dialed
      {
        increment = -5;
      }
      else if (pulse == 4 ) //3 Dialed
      {
        increment = 10;
      }
      else if (pulse == 5 ) //4 Dialed
      {
        increment = -10;
      }
      else if (pulse == 6) //5 Dialed
      {
        increment = 20;
      }
      else if (pulse == 7) //6 Dialed
      {
        increment = -20;
      }
      else if (pulse == 10) //9 Dialed
      {
        degree = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Reset Selected");
        delay(1000);
        SelectMenu();

      }
      else if (pulse == 11) //0 Dialed
      { 
        toggle = 0; 

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Motor Deselected");
        delay(1000);
        DefaultMenu();
      } 
      if (pulse < 10)
      {
        if (AL5D.get_motor_loc(num) + increment > 90)
        {
          degree = 90;
        }
        else if (AL5D.get_motor_loc(num) + increment < -90)
        {
          degree = -90;
        }
        else
        {
          degree = AL5D.get_motor_loc(num) + increment;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Motor: ");
        lcd.print(degree);
        delay(1000);
        SelectMenu();
      }
    }

  else //Dials will select motor to move
    {
      if (pulse == 10) //9 Dialed
      {
        AL5D.center_motors();
        degree = 0;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("All Reset");
        delay(1000);
        DefaultMenu();
      }
      else if (pulse == 11) {}
      else
      {
        num = 0;
        num = pulse - 2;   
        toggle = 1; 
        degree = AL5D.get_motor_loc(num);
        
        SelectMenu();
      }
    } 
  }

  AL5D.set_motor(num, degree, time);
  

  // Write Determined Pulse Value
  if(pulse > 1 && timer == 10)
  {
    pulse = 1;
  }

  // Reset Variable Count when no pulse detected
  if(digitalRead(input) == 0)
  {
    var=0;
  }

  // Reset Timer after 1 seconds
  if(timer==1000)
  {
    pulse = 1;
    timer = 0;
  }
  delay(5); 
}

void DefaultMenu()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("SLCT Motor 1-6,");
  lcd.setCursor(1, 1);
  lcd.print("SPD 7-8, RES 9");
}
void SelectMenu()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SLCT Motor: ");
  lcd.print(num + 1);
  lcd.setCursor(0, 1);
  lcd.print("DSLCT -> 0");
}