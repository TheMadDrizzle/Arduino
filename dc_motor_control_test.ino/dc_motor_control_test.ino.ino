#include "MotorDriver.h"

#define MOTOR_ZERO 0
#define MOTOR_MAX 940
#define MOTOR_MID 500

int j;
long scaledResult;

void setup()
{
  Serial.begin(9600);
  /*Configure the motor A to control the wheel at the left side.*/
  /*Configure the motor B to control the wheel at the right side.*/
  motordriver.init();
  motordriver.setSpeed(200,MOTORB);
  motordriver.setSpeed(200,MOTORA);
}
 
void loop()
{
  int UD = analogRead(A0);
  int LR = analogRead(A1);
  long speed;
  
  if(UD > 500) {
    speed = autoScale(MOTOR_MID, MOTOR_MAX, 0, 200, UD);
    motordriver.setSpeed(speed, MOTORA);
    motordriver.goForward();
  } else if(UD < 500) {
    speed = autoScale(MOTOR_ZERO, MOTOR_MID, 200, 0, UD);
    motordriver.setSpeed(speed, MOTORA);
    motordriver.goBackward();
  } else
    motordriver.stop();
    
/*  if(LR > 500) {
    speed = autoScale(MOTOR_MID, MOTOR_MAX, 0, 200, UD);
    motordriver.setSpeed(speed, MOTORB);
    motordriver.goForward();
  } else if(UD < 500) {
    speed = autoScale(MOTOR_ZERO, MOTOR_MID, 200, 0, UD);
    motordriver.setSpeed(speed, MOTORB);
    motordriver.goBackward();
  } else
    motordriver.stop();
*/

  // SERIAL OUT
  Serial.print("UD: ");
  Serial.print(UD, DEC);
  Serial.print(" SPEED: ");
  Serial.println(speed, DEC);
  delay(200);
}

int autoScale( int originalMin, int originalMax, int newBegin, int newEnd, int inputValue){

  long zeroRefOriginalMax = 0;
  long zeroRefnewEnd = 0;
  long zeroRefCurVal = 0;
  long rangedValue = 0;
  boolean invFlag = 0;

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  zeroRefOriginalMax = originalMax - originalMin;

  if (newEnd > newBegin){
    zeroRefnewEnd = newEnd - newBegin;
  }
  else
  {
    zeroRefnewEnd = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;


 // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

   if (invFlag == 0){
    rangedValue =  ((zeroRefCurVal * zeroRefnewEnd) /
      zeroRefOriginalMax) + newBegin ;
  }
  else     // invert the ranges
  {  
    rangedValue =  newBegin - ((zeroRefCurVal * zeroRefnewEnd) /
      zeroRefOriginalMax)  ;
  }

  return rangedValue;
}
