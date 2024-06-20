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

//enum distance{CannotEvaluate,One,Two,Three,Four,Five,Six, Seven,Eight,Nine};
//distance distanceFromObject = Zero;
#include <math.h>
#define noPaverageseInterrupts 1
// Assignment of the sensor pins
#define S0 A2
#define S1 A1
#define S2 0
#define S3 1
#define sensorOut A0
/*Calibration values (must be updated before
 updatedbefore each use)*/
int redMax = 24390.25;
int redMin = 27776. ;

int blueMax = 17543.;
int blueMin  = 6756.56;

int redColor = 0;

int blueColor = 0;

int redFrequency = 0;
int redEdgeTime = 0;

int blueFrequency = 0;
int blueEdgeTime = 0;

bool redFlag = false;
bool blueFlag = false;

//Initialization of the loop
void evaluatingColour() {
 Serial.println("------------------------------");
 {
 /*Determination of the photodiode type during measurement
 S2/S3
 LOW/LOW=RED, LOW/HIGH=BLUE,
 HIGH/HIGH=GREEN, HIGH/LOW=CLEAR*/
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);

 float(redEdgeTime) = pulseIn(sensorOut, LOW);;
 float(redFrequency) = (1 / (redEdgeTime / 1000000));
 redColor = map(redFrequency, redMax, redMin, 255, 0);
 if (redColor > 255) {
 redColor = 255;
 }
 if (redColor < 0) {
 redColor = 0;
 }
 /*Output of frequency mapped to 0-255*/
 Serial.print("Red Frequency: ");
 Serial.println(redFrequency);
 Serial.print("R = ");
 Serial.println(redColor);
 delay(100);
 }
 {
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 /*Frequency measurement of the specified color and its assignment to an RGB value between 0-255*/
 float(blueEdgeTime) = pulseIn(sensorOut, LOW);
 float(blueFrequency) = (1 / (blueEdgeTime / 1000000));
 blueColor = map(blueFrequency, blueMax, blueMin, 255, 0);
 if (blueColor > 255) {
 blueColor = 255;
 }
 if (blueColor < 0) {
 blueColor = 0;
 }
 /*Output of frequency mapped to 0-255*/
 Serial.print("Blue Frequency: ");
 Serial.println(blueFrequency);
 Serial.print("B = ");
 Serial.println(blueColor);
 delay(100);
 }
 Serial.println("------------------------------");
 delay(100);

if(redColor==255 && blueColor==0){
  Serial.println("No Object Detected");
  delay(200);
}else if(redColor<255 && blueColor==0){
    Serial.println("Red Detected");
    delay(200);
}else if(redColor<=255 && blueColor>0){
    Serial.println("Blue Detected");
    delay(200);
    
}


 Serial.println("------------------------------");
 delay(1000);
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
  delay(1000);  // Adjust the delay for the desired distance
  stop();

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);  // Allow time for turning

  backward(baseSpeed);  // Go backward again
  delay(1000);  // Adjust the delay for the desired distance
  stop();
}

void avoidObstacle() {
  backward(baseSpeed);  // Go backward
  delay(500);  // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);  // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go forward
  delay(1500);  // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnRight45();  // Turn 45 degrees to the right
  delay(500);  // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go forward
  delay(1000);  // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnRight45();  // Turn 45 degrees to the right
  delay(500);  // Allow time for turning
  stop();
  delay(500);  // Short pause

  forward(baseSpeed);  // Go straight
  delay(1000);  // Adjust the delay for the desired distance
  stop();
  delay(500);  // Short pause

  turnLeft45();  // Turn 45 degrees to the left
  delay(500);  // Allow time for turning
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
  long travelTime = pulseIn(echoPin, HIGH,30000);
  return travelTime * 0.034 / 2;  // Distance in mm
}

void obstacleManagement() {
  distanceFromObject = measureDistance();
  if (distanceFromObject<=4 && distanceFromObject>= 5 ){
          stop();
    
    delay(10);
    
    evaluatingColour();


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

 /*definition of the sensor pins*/
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(sensorOut, INPUT);

 digitalWrite(S0, HIGH);
 digitalWrite(S1, LOW);
  
  analogWrite(motorAenable, 255); 
  analogWrite(motorBenable, 255);
}

void loop() {
  int RightInfraRedValue = digitalRead(RightInfraRed);
  int LeftInfraRedValue = digitalRead(LeftInfraRed);

  obstacleManagement();

  if (LeftInfraRedValue == LOW && RightInfraRedValue == LOW) {
    forward(baseSpeed);  // Black line detected, go straight
    //avoidObject();
  } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  // Left sensor off the line, right sensor on the line, turn left
    motorABackward(150);                                                //reduce speed of left motor
    motorBForward(baseSpeed + turnSpeed);                               //increase speed of right motor
  } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  //// Right sensor off the line, left sensor on the line, turn right
    motorAForward(baseSpeed + turnSpeed);                               //increase speed of left motor
    motorBBackward(150);                                                //reduce speed of right motor
  } else {                                                              //error meaning both sensors are not finding a black line, meaning car is off tracks
    backward(baseSpeed);  //reduce speed of right motor
  }
}