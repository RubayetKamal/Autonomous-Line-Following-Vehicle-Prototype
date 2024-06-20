#include <math.h>
const int motorAenable = 11;
const int motorAcontrolA = 10;
const int motorAcontrolB = 9;

const int motorBcontrolA = 8;
const int motorBcontrolB = 7;
const int motorBenable = 6;

const int LeftInfraRed = 2;
const int RightInfraRed = 3;

const int triggerPin = 4;
const int echoPin = 5;
float distanceFromObject = 5000;

int baseSpeed = 100;
int turnSpeed = 70;

const int colourDetectingPinOne = 0;  //colourDetectingPinOne
const int colourDetectingPinTwo = 1;  //colourDetectingPinTwo

const int frequencyDetectingPinOne = A2;  //S0
const int frequencyDetectingPinTwo = A1;  //S1

const int colourSensorOutput = A0;  //colourSensorOutput
const int colourSensorLED = A3;

bool redFlag = false;
bool blueFlag = false;

int redMin = 0;
int redMax = 1;

int blueMin = 0;
int blueMax = 1;

int redColor = 0;
int blueColor = 0;

float redFrequency = 0.;
float redEdgeTime = 0.;

float blueFrequency = 0.;
float blueEdgeTime = 0.;

void checkingRed() {
  digitalWrite(colourDetectingPinOne, LOW);  //Red Filter Set
  digitalWrite(colourDetectingPinTwo, LOW);
  redEdgeTime = pulseIn(colourSensorOutput, HIGH) + pulseIn(colourSensorOutput, LOW);  //Period of Red
  redFrequency = (1 / (redEdgeTime / 1000000));                                        //frequency of Red
  redColor = map(redFrequency, redMax, redMin, 255, 0);                                //mapped frequency
  redColor = constrain(redColor, 0, 255);                                              //ensures it does not go outside 0 and 255
  Serial.print("Red Frequency: ");
  Serial.println(redFrequency);
  Serial.print("R = ");
  Serial.println(redColor);
  delay(100);
}

void checkingBlue() {
  digitalWrite(colourDetectingPinOne, LOW);
  digitalWrite(colourDetectingPinTwo, HIGH);
  /*Frequency measurement of the specified color and its assignment to an RGB value between 0-255*/
  blueEdgeTime = pulseIn(colourSensorOutput, HIGH) + pulseIn(colourSensorOutput, LOW);
  blueFrequency = (1 / (blueEdgeTime / 1000000));
  blueColor = map(blueFrequency, blueMax, blueMin, 255, 0);
  blueColor = constrain(blueColor, 0, 255);

  Serial.print("Blue Frequency: ");
  Serial.println(blueFrequency);
  Serial.print("B = ");
  Serial.println(blueColor);
  delay(100);
}

void determiningColour() {

  //
}

void evaluatingColour() {
  Serial.println("------------------------------");
  checkingRed();
  checkingBlue();
  Serial.println("------------------------------");
  delay(100);
  determiningColour();
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
  return travelTime * 0.34 / 2;  // Distance in mm
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
void backward() {
  motorABackward(baseSpeed);
  motorBBackward(baseSpeed);
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
  delay(1000);
  stop();
}
void turnLeft(int leftWheelBackSpeed) {
  motorABackward(leftWheelBackSpeed);    //reduce speed of left motor
  motorBForward(baseSpeed + turnSpeed);  //increase speed of right motor
}

void turnRight(int rightWheelBackSpeed) {
  motorAForward(baseSpeed + turnSpeed);  //increase speed of left motor
  motorBBackward(rightWheelBackSpeed);   //reduce speed of right motor
}

void goAround() {
  stop();
  delay(500);

  turnRight(baseSpeed);
  delay(500);  //to adjust for desired turn angle
  stop();
  delay(500);

  forward(baseSpeed);
  delay(1000);  //adjust
  stop();
  delay(500);

  turnLeft(baseSpeed);
  delay(500);  //adjust
  stop();
  delay(500);

  forward(baseSpeed);
  delay(1000);  //adjust
  stop();
  delay(500);

  // Turn left to re-align with the original path
  turnLeft(baseSpeed);
  delay(500);  // Adjust this delay based on your motor speed and desired turn angle
  stop();
  delay(500);

  // Move forward to continue on the original path
  forward(baseSpeed);
  delay(1000);  // Adjust this delay based on the size of the object
  stop();
  delay(500);

  // Turn right to re-align with the original path
  turnRight(baseSpeed);
  delay(500);  // Adjust this delay based on your motor speed and desired turn angle
  stop();
  delay(500);
}

void avoidObject() {
  distanceFromObject = measureDistance();
  if (distanceFromObject <= 400.) {
    stop();
    delay(10);
    
    evaluatingColour();
    if (redFlag) {
      turn180();
      delay(10);
      redFlag = false;
    }
    if (blueFlag) {
      goAround();
      delay(10);
      blueFlag = false;
    }

  }
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


  pinMode(frequencyDetectingPinOne, OUTPUT);
  pinMode(frequencyDetectingPinTwo, OUTPUT);
  pinMode(colourDetectingPinOne, OUTPUT);
  pinMode(colourDetectingPinTwo, OUTPUT);

  pinMode(colourSensorOutput, INPUT);

  digitalWrite(frequencyDetectingPinOne, HIGH);
  digitalWrite(frequencyDetectingPinTwo, HIGH);
  //pinMode(RightInfraRed, INPUT_PULLUP);

  analogWrite(motorAenable, 255);  //inital speed of Motors
  analogWrite(motorBenable, 255);

  pinMode(colourSensorLED, OUTPUT);
  digitalWrite(colourSensorLED, HIGH);
}


void loop() {
  int RightInfraRedValue = digitalRead(RightInfraRed);
  int LeftInfraRedValue = digitalRead(LeftInfraRed);

  avoidObject();

  if (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH) {
    forward(baseSpeed);  // Black line detected, go straight //it was 150
    //avoidObject();
  } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  // Left sensor off the line, right sensor on the line, turn left
    turnLeft(150);
  } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  //// Right sensor off the line, left sensor on the line, turn right
    turnRight(150);
  } else {  //error meaning both sensors are not finding a black line, meaning car is off tracks
    //stop(); //reduce speed of right motor
  }
}
