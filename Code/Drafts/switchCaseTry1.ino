#include <math.h>

const int motorAenable = 11;
const int motorAcontrolA = 10;
const int motorAcontrolB = 9;

const int motorBcontrolA = 8;
const int motorBcontrolB = 7;
const int motorBenable = 6;

const int LeftInfraRed = 2;
const int RightInfraRed = 3;

int RightInfraRedValue;
int LeftInfraRedValue;

const int triggerPin = 4;
const int echoPin = 5;
float distanceFromObject = 5000;

int baseSpeed = 125;
int turnSpeed = 70;

enum State { Idle,
             GoingForward,
             TurningRight,
             TurningLeft,
             GoingBackward,
             Stopped,
             ObstacleDetected,
             EvaluatingColour,
             AvoidingObstacle,
             Parking };
State activeState = GoingForward;

#define S0 A2
#define S1 A1
#define S2 0
#define S3 1
#define sensorOut A0
#define lights A3

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redValue;
int greenValue;
int blueValue;

bool redFlag = false;
bool greenFlag = false;
bool blueFlag = false;

void evaluatingColour() {

  redPW = getRedPW();
  delay(200);
  // greenPW = getGreenPW();
  // delay(200);
  bluePW = getBluePW();
  delay(200);

  Serial.print("Red = ");
  Serial.print(redPW);
  Serial.print(" - Blue = ");
  Serial.println(bluePW);

  // if (redPW >= 92 && redPW <= 510 && greenPW >= 105 && greenPW <= 470 && bluePW >= 99 && bluePW <= 466) {
  //   Serial.println("RedFound");
  //   redFlag = true;
  // } else if (redPW >= 96 && redPW <= 1120 && greenPW >= 99 && greenPW <= 1126 && bluePW >= 95 && bluePW <= 1127) {
  //   Serial.println("BlueFound");
  //   blueFlag = true;
  // } else {
  //   Serial.println("Unknown");
  // }
}



int getRedPW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int PW;
  PW = pulseIn(sensorOut, HIGH, 20000);
  return PW;
}

int getGreenPW() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int PW;
  PW = pulseIn(sensorOut, HIGH, 20000);
  return PW;
}

int getBluePW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int PW;
  PW = pulseIn(sensorOut, HIGH, 20000);
  return PW;
}

void motorAForward(int speedA) {
  analogWrite(motorAenable, speedA);
  digitalWrite(motorAcontrolA, LOW);
  digitalWrite(motorAcontrolB, HIGH);
}

void motorBForward(int speedB) {
  analogWrite(motorBenable, speedB);
  digitalWrite(motorBcontrolA, HIGH);
  digitalWrite(motorBcontrolB, LOW);
}

void motorABackward(int speedA) {
  analogWrite(motorAenable, speedA);
  digitalWrite(motorAcontrolA, HIGH);
  digitalWrite(motorAcontrolB, LOW);
}

void motorBBackward(int speedB) {
  analogWrite(motorBenable, speedB);
  digitalWrite(motorBcontrolA, LOW);
  digitalWrite(motorBcontrolB, HIGH);
}

void backward(int speed) {
  motorABackward(speed);
  motorBBackward(speed);
}

void motorAStop() {
  analogWrite(motorAenable, 0);
}

void motorBStop() {
  analogWrite(motorBenable, 0);
}

void forward(int carSpeed) {
  motorAForward(carSpeed);
  motorBForward(carSpeed);
}

void stop() {
  motorAStop();
  motorBStop();
}

void turn180() {
  motorABackward(baseSpeed);
  motorBForward(baseSpeed);
  delay(1000);  // Adjust this delay based on your motor speed and desired turn time
  stop();
}

void decelerate() {
  for (int i = baseSpeed; i > 0; i -= 20) {
    forward(i);
    delay(100);
  }
  stop();
}

void parkCar() {
  backward(baseSpeed);  // Go backward
  delay(500);          // Adjust the delay for the desired distance
  stop();

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);    // Allow time for turning

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);    // Allow time for turning

  backward(baseSpeed);  // Go backward again
  delay(1000);          // Adjust the delay for the desired distance
  stop();
}

void avoidObstacle() {
  backward(baseSpeed);  // Go backward
  delay(500);           // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);    // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go forward
  delay(1500);         // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnRight45();  // Turn 45 degrees to the right
  delay(500);     // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go forward
  delay(1000);         // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnRight45();  // Turn 45 degrees to the right
  delay(500);     // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go straight
  delay(1000);         // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);    // Allow time for turning
  stop();
}

// Function to turn right by 45 degrees
void turnRight45() {
  motorBForward(baseSpeed);
  motorABackward(baseSpeed);
  delay(250);  // Adjust this delay based on your motor speed and desired turn time for a 45 degree turn
  stop();
}

// Function to turn left by 45 degrees
void turnLeft45() {
  motorAForward(baseSpeed);
  motorBBackward(baseSpeed);
  delay(250);  // Adjust this delay based on your motor speed and desired turn time for a 45 degree turn
  stop();
}

void activateTriggerPin() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

float measureDistance() {
  activateTriggerPin();
  long travelTime = pulseIn(echoPin, HIGH, 30000);
  return travelTime * 0.034 / 2;  // Distance in mm
}

void obstacleManagement() {
  distanceFromObject = measureDistance();
  if (distanceFromObject <= 10. && distanceFromObject >= 1.) {
    //Serial.println(distanceFromObject);
    activeState = Stopped;
  }
}

void InfraRedManagement(){
     RightInfraRedValue = digitalRead(RightInfraRed);
   LeftInfraRedValue = digitalRead(LeftInfraRed);
}


void setup() {
  Serial.begin(9600);

  pinMode(motorAcontrolA, OUTPUT);
  pinMode(motorAcontrolB, OUTPUT);
  pinMode(motorBcontrolA, OUTPUT);
  pinMode(motorBcontrolB, OUTPUT);
  pinMode(motorAenable, OUTPUT);
  pinMode(motorBenable, OUTPUT);

  pinMode(LeftInfraRed, INPUT);
  pinMode(RightInfraRed, INPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(lights, OUTPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  digitalWrite(lights, HIGH);

  analogWrite(motorAenable, 255);
  analogWrite(motorBenable, 255);
}

void loop() {

  InfraRedManagement();
  obstacleManagement();

  switch (activeState) {
    case GoingForward:
      {
        forward(baseSpeed);
        if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  // Left sensor off the line, right sensor on the line, turn left
          activeState = TurningLeft;
          break;
        } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  //// Right sensor off the line, left sensor on the line, turn right
          activeState = TurningRight;
          break;
        } else if (RightInfraRedValue == LOW && LeftInfraRedValue == LOW) {
          activeState = GoingBackward;
          break;
        }else{
          forward(baseSpeed);
        }
      }
    case TurningLeft:
      {
        motorABackward(120);                   //reduce speed of left motor
        motorBForward(baseSpeed + turnSpeed);  //increase speed of right motor
        activeState = GoingForward;
        break;
      }
    case TurningRight:
      {
        motorAForward(baseSpeed + turnSpeed);  //increase speed of left motor
        motorBBackward(120);                   //reduce speed of right motor
        activeState = GoingForward;
        break;
      }
    case Stopped:
      {
        stop();
        delay(10);
        evaluatingColour();
        delay(100);
        activeState = EvaluatingColour;
        break;
      }
    case GoingBackward:
      {
        backward(baseSpeed);
        delay(100);
        activeState = GoingForward;
        break;
      }
    case Idle:
      {
        stop();
        if(RightInfraRedValue == HIGH && LeftInfraRedValue == HIGH){
          activeState = GoingForward;
          break;
        }
      }
    case EvaluatingColour:
      {
        if (redFlag) {
          activeState = AvoidingObstacle;
          break;
        } else if (blueFlag) {
          activeState = Parking;
          break;
        } else {
          activeState = GoingForward;
          break;
        }
      }
    case AvoidingObstacle:
      {
        avoidObstacle();
        activeState = GoingForward;
        break;
      }
    case Parking:
      {
        parkCar();
        activeState = Idle;
        break;
      }
  }
}