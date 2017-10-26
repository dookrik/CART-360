/*
  code for riping off limbs and getting a response!
  the point of this code is to notify a pin when a circuit is broken and which will result in the rumble
  of a servo.
*/

//library
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#define SERVO_PIN_2 2
#define SERVO_PIN_3 3
#define SERVO_PIN_4 4
#define SERVO_PIN_5 5

//to test the current flow
//int ledPin = 13;
//byte leds = 0;
// think of these as temp switches allowing power to flow only in one way
// switches that act as legs
int btnAservoP5 = 6;
int btnBservoP2 = 7;
//int btnCservoP3 = 8;
//int btnDservoP4 = 9;

void setup()
{
  Serial.begin(9600);
  //power my switches (legs) through the limbs so it does the opposite  of a nnormal switch
  pinMode(ledPin, OUTPUT);
  pinMode(btnAservoP5, INPUT_PULLUP);
  pinMode(btnBservoP2, INPUT_PULLUP);
  pinMode(btnCservoP3, INPUT_PULLUP);
  pinMode(btnDservoP4, INPUT_PULLUP);

  // references for servos
  servo1.attach(SERVO_PIN_5); // servo 1
  servo2.attach(SERVO_PIN_2); // servo 2
  servo3.attach(SERVO_PIN_3); //servo 3
  servo4.attach(SERVO_PIN_4); //servo 4
  /*
    // test that everything is running
    for (int i = 0; i < 100; i++) {
     servo2.write(i);
      servo1.write(i);

     delay(50);
    }

    digitalWrite(ledPin,HIGH);
    delay(1000);
    digitalWrite(ledPin,LOW);
  */
}

void loop()
{
  // A button. when disconected act=============================================================
// servo 1 is actually refered to pin 5 !!!!!!!!!!!!!!!
  if (digitalRead(btnAservoP5) == HIGH)
  {
    //digitalWrite(ledPin, HIGH);
    //loop through and wiggle!
    for (int i = 0; i < 60; i++)
    {
      //this is on =========================================== pin 3
      servo1.write(i);
      delay(15);
    }

  }
  // when connect dont act, go back to original position and stay at 0 deg..
  if (digitalRead(btnAservoP5) == LOW)
  {
   // digitalWrite(ledPin, LOW);
    for (int i = 60; i <= 0; i--)
    {
      servo1.write(i);
      delay(15);
    }
    digitalWrite(ledPin, LOW);
    // delay(1000);
  }
  //check data
  //   Serial.println(SERVO_PIN_5);

  // B button connected to servo 2===================================================
  Serial.println(digitalRead(btnBservoP2));
  if (digitalRead(btnBservoP2) == HIGH)
  {
    //loop through and wiggle!
    for (int i = 0; i < 60; i++)
    {
      //this is on pin 2
      servo2.write(i);
      delay(15);
    }

  }
  // when connect dont act, go back to original position and stay at 0 deg..
  if (digitalRead(btnBservoP2) == LOW)
  {

    for (int i = 60; i <= 0; i--)
    {
      servo2.write(i);
      delay(15);
    }
    digitalWrite(ledPin, LOW);
    delay(1000);
  }

  // C button connected to servo 1 ===================================================
  /*
    if (digitalRead(btnCservoP3) == HIGH)
    {
      //loop through and wiggle!
      for (int i = 0; i < 100; i++)
      {
        //this is on pin 5
        servo3.write(i);
        delay(15);
      }
    }
    // when connect dont act, go back to original position and stay at 0 deg..
    if (digitalRead(btnCservoP3) == LOW)
    {

      for (int i = 100; i <= 0; i--)
      {
        servo3.write(i);
        delay(15);
      }
      digitalWrite(ledPin, LOW);
      delay(1000);
    }

    //D button and servo 4 ==============================================================

     if (digitalRead(btnDservoP4) == HIGH)
    {
      //loop through and wiggle!
      for (int i = 0; i < 100; i++)
      {
        //this is on====================== servo pin 4
        servo4.write(i);
        delay(15);
      }
    }
    // when connect dont act, go back to original position and stay at 0 deg..
    if (digitalRead(btnDservoP4) == LOW)
    {

      for (int i = 100; i <= 0; i--)
      {
        servo4.write(i);
        delay(15);
      }
    }
  */
}


