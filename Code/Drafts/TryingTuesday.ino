const int motorAenable = 11;
const int motorAcontrolA = 10;
const int motorAcontrolB = 9;

const int motorBcontrolA = 8;
const int motorBcontrolB = 7;
const int motorBenable = 6;

const int LeftInfraRed = 2;
const int RightInfraRed = 3;

int baseSpeed = 150;
int turnSpeed = 80;

const int triggerPin = 4;
const int echoPin = 5;
float distanceFromObject;

enum State { FollowingLine, ObstacleDetected };
enum LineType { Straight, TurningLeft, TurningRight };
State activeState = FollowingLine;
LineType line = Straight;

int RightInfraRedValue;
int LeftInfraRedValue;

// Motor control functions
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

void activateTriggerPin() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

float measureDistance() {
  activateTriggerPin();
  long travelTime = pulseIn(echoPin, HIGH);
  return travelTime * 0.34 / 2;  // Distance in mm
}

void readInfraRed() {
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

  analogWrite(motorAenable, 150);  // Initial speed of Motors
  analogWrite(motorBenable, 150);
}

void avoidObject() {
  distanceFromObject = measureDistance();
  Serial.println(distanceFromObject);
  if (distanceFromObject <= 200) {
    activeState = ObstacleDetected;
  } else {
    activeState = FollowingLine;
  }
}

void turnRight() {
  motorAForward(baseSpeed + turnSpeed);
  motorBForward(baseSpeed - turnSpeed);
}

void turnLeft() {
  motorAForward(baseSpeed - turnSpeed);
  motorBForward(baseSpeed + turnSpeed);
}

void goBackward() {
  motorABackward(baseSpeed);
  motorBBackward(baseSpeed);
}

void loop() {
  readInfraRed();
  avoidObject();

  switch (activeState) {
    case FollowingLine: {
      if (RightInfraRedValue == HIGH && LeftInfraRedValue == LOW) {
        line = TurningLeft;
      } else if (RightInfraRedValue == LOW && LeftInfraRedValue == HIGH) {
        line = TurningRight;
      } else {
        line = Straight;
      }

      switch (line) {
        case Straight:
          forward(baseSpeed);
          break;
        case TurningLeft:
          turnLeft();
          line = Straight;
          break;
        case TurningRight:
          turnRight();
          line = Straight;
          break;
      }
      break;
    }
    case ObstacleDetected:
      stop();
      delay(100);  // Delay to prevent infinite loop
      if (distanceFromObject > 200) {
        activeState = FollowingLine;
      }
      break;
  }

  delay(50);  // Small delay to make the system more stable
}
