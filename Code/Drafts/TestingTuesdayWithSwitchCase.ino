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

int RightInfraRedValue;
int LeftInfraRedValue;

enum State { NoLineDetected,
             LineDetectedLeft,
             LineDetectedRight,
             ObjectDetected };
State activeState = NoLineDetected;

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

void forward(int carSpeed) {
  motorAForward(carSpeed);
  motorBForward(carSpeed);
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

void checkingInfraRed(){
  RightInfraRedValue = digitalRead(RightInfraRed);
  LeftInfraRedValue = digitalRead(LeftInfraRed);
}

void stop() {
  motorAStop();
  motorBStop();
}

void setup() {
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

  analogWrite(motorAenable, 255);  //inital speed of Motors
  analogWrite(motorBenable, 255);
}

void avoidObject() {
  distanceFromObject = measureDistance();
  if (distanceFromObject <= 200) {
    activeState = ObjectDetected;
    delay(10);
  }
}

void loop() {
  avoidObject();  // Check for object detection first

  checkingInfraRed();
  
  switch (activeState) {
    case NoLineDetected: {
      forward(150);
      if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  // Left sensor off the line, right sensor on the line, turn left
        activeState = LineDetectedLeft;
      }
      if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  // Right sensor off the line, left sensor on the line, turn right
        activeState = LineDetectedRight;
      }
      break;
    }
    case LineDetectedLeft: {
      motorABackward(150);                   // Reduce speed of left motor
      motorBForward(baseSpeed + turnSpeed);  // Increase speed of right motor
      if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {
        activeState = LineDetectedRight;
      } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH) {
        activeState = NoLineDetected;
      }
      break;
    }
    case LineDetectedRight: {
      motorAForward(baseSpeed + turnSpeed);  // Increase speed of left motor
      motorBBackward(150);                   // Reduce speed of right motor
      if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {
        activeState = LineDetectedLeft;
      } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH) {
        activeState = NoLineDetected;
      }
      break;
    }
    case ObjectDetected: {
      stop();
      if (distanceFromObject > 200) {
        activeState = NoLineDetected;
      }else if(distanceFromObject<=200){
        stop();
      }else{
        forward(baseSpeed);
      }
      break;
    }
    default: {
      forward(baseSpeed);
      break;
    }
  }
}
