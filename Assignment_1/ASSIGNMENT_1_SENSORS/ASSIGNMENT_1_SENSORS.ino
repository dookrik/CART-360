/* This code is used to give life and sensitivity to the octopus... do not squeeze it too hard Qa Q !!! it will wiggle out in pain.
 *  takes in analog input from a velostat pressure sensor and uses it to measure when a press of a certain threshold is met and reacts accordingly

*/
// separate code from other arduino
#include <Servo.h>
#define SENSOR_PIN_1 A0
#define SENSOR_PIN_2 A1
#define SENSOR_PIN_3 A2
#define SENSOR_PIN_4 A3

Servo servo1;   
Servo servo2;   
Servo servo3;   
Servo servo4;   

#define SERVO_PIN_1 2
#define SERVO_PIN_2 3
#define SERVO_PIN_3 4
#define SERVO_PIN_4 5

// medium treshold == ok for octopus
int thresholdOk = 400;
// high value == pain for octopus
int thresholdOuch = 900;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
 pinMode(3, INPUT);     //force sensor value input
 servo1.attach(SERVO_PIN_1);
 servo2.attach(SERVO_PIN_2);
 servo3.attach(SERVO_PIN_3);
 servo4.attach(SERVO_PIN_4);
  
}

void loop() {
  
 //these read the values from our 
   int sensorPin_1 = analogRead(A0);
   int sensorPin_2 = analogRead(A1);
   int sensorPin_3 = analogRead(A2);
   int sensorPin_4 = analogRead(A3);
// 1st sensor ===========================================================================================================================

 //if the values from the sensor are higher or equal to the acceptable threshold(soft) vibrate for as long as it has this condition 
 if(sensorPin_1 >= thresholdOk) {
    for(int i = 0; i < 50; i++){
      servo1.write(i);
      delay(15);  
      Serial.println(sensorPin_1); 
   }
    //reverse action of prev statement when false and stay at 0 deg.
  } else if(sensorPin_1 < thresholdOk) {
    for(int i = 50; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
  }
//-=-=-= higher threshold -=-=-=-=-=-= short sweeps constant for as long as it is being pressed
  if(sensorPin_1 >= thresholdOuch) {
    for(int i = 0; i < 5; i++){
      servo1.write(i);
      delay(15);
    }
   // this returns false which means either under thresholdOk or under thresholdOuch 
   // this is a problem because  if its not == to 5, it will decrement from a higher sweep value and only decrement by 5
 } else if(sensorPin_1 < thresholdOuch) {
    for(int i = 5; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
   }
   ////-=-=--=-= 2nd sensor-========================================================================================================

   
 //if the values from the sensor are higher or equal to the acceptable threshold(soft) vibrate for as long as it has this condition 
 if(sensorPin_2 >= thresholdOk) {
    for(int i = 0; i < 50; i++){
      servo1.write(i);
      delay(15);  
     // Serial.println(sensorPin_2); 
   }
    //reverse action of prev statement when false and stay at 0 deg.
  } else if(sensorPin_2 < thresholdOk) {
    for(int i = 50; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
  }
//-=-=-= higher threshold -=-=-=-=-=-= short sweeps constant for as long as it is being pressed
  if(sensorPin_2 >= thresholdOuch) {
    for(int i = 0; i < 5; i++){
      servo1.write(i);
      delay(15);
    }
   // this returns false which means either under thresholdOk or under thresholdOuch 
   // this is a problem because  if its not == to 5, it will decrement from a higher sweep value and only decrement by 5
 } else if(sensorPin_2 < thresholdOuch) {
    for(int i = 5; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
   }
   // 3rd sensor===================================================================================================================
   
    //if the values from the sensor are higher or equal to the acceptable threshold(soft) vibrate for as long as it has this condition 
 if(sensorPin_3 >= thresholdOk) {
    for(int i = 0; i < 50; i++){
      servo1.write(i);
      delay(15);  
     // Serial.println(sensorPin_3); 
   }
    //reverse action of prev statement when false and stay at 0 deg.
  } else if(sensorPin_3 < thresholdOk) {
    for(int i = 50; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
  }
//-=-=-= higher threshold -=-=-=-=-=-= short sweeps constant for as long as it is being pressed
  if(sensorPin_3 >= thresholdOuch) {
    for(int i = 0; i < 5; i++){
      servo1.write(i);
      delay(15);
    }
   // this returns false which means either under thresholdOk or under thresholdOuch 
   // this is a problem because  if its not == to 5, it will decrement from a higher sweep value and only decrement by 5
 } else if(sensorPin_3 < thresholdOuch) {
    for(int i = 5; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
   }
   // 4th sensor =======================================================================================================================
   
    //if the values from the sensor are higher or equal to the acceptable threshold(soft) vibrate for as long as it has this condition 
 if(sensorPin_4 >= thresholdOk) {
    for(int i = 0; i < 50; i++){
      servo1.write(i);
      delay(15);  
      //Serial.println(sensorPin_4); 
   }
    //reverse action of prev statement when false and stay at 0 deg.
  } else if(sensorPin_4 < thresholdOk) {
    for(int i = 50; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
  }
//-=-=-= higher threshold -=-=-=-=-=-= short sweeps constant for as long as it is being pressed
  if(sensorPin_4 >= thresholdOuch) {
    for(int i = 0; i < 5; i++){
      servo1.write(i);
      delay(15);
    }
   // this returns false which means either under thresholdOk or under thresholdOuch 
   // this is a problem because  if its not == to 5, it will decrement from a higher sweep value and only decrement by 5
 } else if(sensorPin_4 < thresholdOuch) {
    for(int i = 5; i <= 0; i--){
      servo1.write(i);
      delay(15);
    }
   } 

  // loop bracket dont remove > A <
 }



