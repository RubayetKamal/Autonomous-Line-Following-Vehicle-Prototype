#include <math.h>
#define noPaverageseInterrupts 1
// Assignment of the sensor pins
#define S0 A2
#define S1 A1
#define S2 0
#define S3 1
#define sensorOut A0


int redMax = 24390.25;
int redMin = 27776.;

int blueMax = 17543.;
int blueMin = 6756.56;


int redColor = 0;
int blueColor = 0;

int redFrequency = 0;
int redEdgeTime = 0;

int blueFrequency = 0;
int blueEdgeTime = 0;

bool redFlag = false;
bool blueFlag = false;

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

int baseSpeed = 120;
int turnSpeed = 70;

int RightInfraRedValue = 1;
int LeftInfraRedValue = 1;

enum Direction { Straight,
                 Left,
                 Right };
Direction currentDirection = Straight;

enum Colour { Red,
              Blue,
              Unknown,
              NoObject };
Colour colourIdentified = NoObject;

enum State { Moving,
             Stopped,
             Idle };
State currentState = Moving;

enum LineType {
  WhiteLine,
  BlackLine
};
LineType lineType = WhiteLine;


void evaluatingColour() {
  {
    ResetColourFlags();

    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);

    float(redEdgeTime) = pulseIn(sensorOut, LOW);
    float(redFrequency) = (1 / (redEdgeTime / 1000000));
    redColor = map(redFrequency, redMax, redMin, 255, 0);
    if (redColor > 255) {
      redColor = 255;
    }
    if (redColor < 0) {
      redColor = 0;
    }
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
    delay(100);
  }
  delay(100);

  if (redColor == 255 && blueColor == 0) {
    Serial.println("No Object Detected");
    delay(200);
  } else if (redColor < 255 && blueColor == 0) {
    Serial.println("Red Detected");
    delay(200);
    blueFlag = false;
    redFlag = true;
  } else if (redColor <= 255 && blueColor > 0) {
    Serial.println("Blue Detected");
    delay(200);
    redFlag = false;
    blueFlag = true;
  }
  delay(1000);
}

void ResetColourFlags() {
  redFlag = false;
  blueFlag = false;
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

void stop() {
  motorAStop();
  motorBStop();
}

void turn180() {
  motorABackward(baseSpeed);
  motorBForward(baseSpeed);
  delay(1000);
  switch (lineType) {
    case WhiteLine:
      {
        do {
          motorABackward(baseSpeed);
          motorBForward(baseSpeed);
          InfraRedManager();

        } while (RightInfraRedValue == HIGH && LeftInfraRedValue == HIGH);
      }
    case BlackLine:
      {
        do {
          motorABackward(baseSpeed);
          motorBForward(baseSpeed);
          InfraRedManager();

        } while (RightInfraRedValue == LOW && LeftInfraRedValue == LOW);
      }
    default:
      {
        break;
      }
  }
  stop();
  delay(10);
}

void turnLeft45() {
  motorABackward(180);
  motorBForward(180);
  delay(200);  // Adjust the delay to achieve a 45-degree turn
  stop();
  delay(1000);
}

void turnLeft90() {
  motorBForward(180);
  motorABackward(180);
  delay(300);  // Adjust the delay to achieve a 90-degree turn
  stop();
  delay(1000);
}

void turnRight45() {
  motorBBackward(180);
  motorAForward(180);
  delay(200);  // Adjust the delay to achieve a 45-degree turn
  stop();
  delay(1000);
}

void turnRight90() {
  motorAForward(180);
  motorBBackward(180);
  delay(400);  // Adjust the delay to achieve a 90-degree turn
  stop();
  delay(1000);
}

void goStraightLong() {
  forward(baseSpeed);
  delay(2500);  // Adjust the delay to move a short distance
  stop();
  delay(1500);
}

void goStraightShort() {
  switch (lineType) {
    case WhiteLine:
      {
        do {
          forward(baseSpeed);
          InfraRedManager();
        } while (RightInfraRedValue == HIGH && LeftInfraRedValue == HIGH);
        break;
      }
    case BlackLine:
      {
        do {
          forward(baseSpeed);
          InfraRedManager();
        } while (RightInfraRedValue == LOW && LeftInfraRedValue == LOW);
        break;
      }
    default:
      {
        break;
      }
  }
  stop();
  delay(500);
  currentState = Moving;
}

void avoidObject() {
  distanceFromObject = measureDistance();
  Serial.println(distanceFromObject);

  if (distanceFromObject >= 10. && distanceFromObject <= 55.) {

    currentState = Stopped;
  }
}

void overtake() {
  turnLeft45();

  distanceFromObject = measureDistance();
  if (distanceFromObject >= 10 && distanceFromObject < 200) {

    stop();
    delay(10);

    backward(baseSpeed);
    delay(600);

    turnRight90();
    goStraightLong();

    motorBForward(255);
    motorABackward(255);
    delay(200);  // Adjust the delay to achieve a 90-degree turn

    stop();
    delay(1000);

    goStraightShort();

  } else {
    goStraightLong();

    motorAForward(255);
    motorBBackward(255);
    delay(200);  // Adjust the delay to achieve a 90-degree turn

    stop();
    delay(1000);

    goStraightShort();
  }
  // turnLeft90();
}



void parkCar() {
  // Turn 90 degrees to the left
  motorAForward(180);
  motorBBackward(180);
  delay(500);  // Adjust the delay to achieve a 90-degree turn
  stop();
  delay(1000);

  // Go back a little bit
  motorABackward(200);
  motorBBackward(220);
  delay(500);  // Adjust the delay to move back a little bit

  currentState = Idle;
}


void InfraRedManager() {
  RightInfraRedValue = digitalRead(RightInfraRed);
  LeftInfraRedValue = digitalRead(LeftInfraRed);
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

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  /*Scaling the output frequency
 S0/S1
 LOW/LOW=AUS, LOW/HIGH=2%,
 HIGH/LOW=20%, HIGH/HIGH=100%*/
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Serial.begin(9600);
}

void loop() {

  switch (lineType) {
    case WhiteLine:
      {
        switch (currentState) {
          case Moving:
            {
              InfraRedManager();
              switch (currentDirection) {
                case Straight:
                  {
                    avoidObject();
                    if (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH) {
                      forward(baseSpeed);  // Black line detected, go straight
                      currentDirection = Straight;
                    } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {
                      currentDirection = Left;  // Turn left
                    } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {
                      currentDirection = Right;  // Turn right
                    } else {
                      //turnLeft45();  // Both sensors off the line, go backward

                      backward(baseSpeed);
                      delay(50);

                      motorBForward(255);
                      motorABackward(255);
                      delay(500);  // Adjust the delay to achieve a 240-degree turn

                      do {
                        InfraRedManager();
                        motorBForward(80);
                        motorABackward(80);
                        if (LeftInfraRedValue == LOW) {
                          currentDirection = Left;
                          break;
                        }
                      } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);
                    }
                    break;
                  }
                case Right:
                  {
                    avoidObject();
                    motorAForward(baseSpeed + turnSpeed);  // Increase speed of left motor
                    motorBBackward(150);                   // Reduce speed of right motor
                    currentDirection = Straight;
                    break;
                  }
                case Left:
                  {
                    avoidObject();
                    motorABackward(150);                   // Reduce speed of left motor
                    motorBForward(baseSpeed + turnSpeed);  // Increase speed of right motor
                    currentDirection = Straight;
                    break;
                  }
                default:
                  {
                    currentDirection = Straight;  // Default to going backward if no valid direction
                    break;
                  }
              }
              break;  // Added break for the Moving case
            }


          case Stopped:
            {
              stop();
              delay(2000);

              evaluatingColour();
              evaluatingColour();
              evaluatingColour();

              if (redFlag) {
                colourIdentified = Red;
              } else if (blueFlag) {
                colourIdentified = Blue;
              } else {
                colourIdentified = Unknown;
              }

              switch (colourIdentified) {
                case Red:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    overtake();
                    delay(10);

                    currentState = Moving;
                    break;  // Added break for the Red case
                  }
                case Blue:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    parkCar();
                    delay(10);

                    currentState = Idle;
                    break;  // Added break for the Blue case
                  }
                case Unknown:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    turn180();
                    delay(10);

                    currentState = Moving;
                    break;  // Added break for the Unknown case
                  }
                default:
                  {
                    currentState = Moving;
                    break;
                  }
              }
              break;  // Added break for the Stopped case
            }


          case Idle:
            {

              do {
                stop();
                delay(10);

                InfraRedManager();

                distanceFromObject = measureDistance();
                if (distanceFromObject >= 10 && distanceFromObject < 200) {
                  turnRight45();
                  break;
                }
              } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);
              do {
                forward(100);
              } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);

              backward(baseSpeed);
              delay(50);

              motorBForward(255);
              motorABackward(255);
              delay(500);  // Adjust the delay to achieve a 240-degree turn

              do {
                InfraRedManager();
                motorBForward(80);
                motorABackward(80);
                if (LeftInfraRedValue == LOW) {
                  currentDirection = Left;
                  currentState = Moving;
                  break;
                }
              } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);
              break;
            }
          default:
            {
              // Handle any unexpected states for currentState
              break;
            }
        }
        break;  // Added break for the WhiteLine case
      }








    case BlackLine:
      {
        switch (currentState) {
          case Moving:
            {
              InfraRedManager();
              switch (currentDirection) {
                case Straight:
                  {
                    avoidObject();
                    if (LeftInfraRedValue == LOW && RightInfraRedValue == LOW) {
                      forward(baseSpeed);  // Black line detected, go straight
                      currentDirection = Straight;
                    } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {
                      currentDirection = Left;  // Turn left
                    } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {
                      currentDirection = Right;  // Turn right
                    } else {
                      //turnLeft45();  // Both sensors off the line, go backward

                      backward(baseSpeed);
                      delay(50);

                      motorBForward(255);
                      motorABackward(255);
                      delay(500);  // Adjust the delay to achieve a 240-degree turn

                      do {
                        InfraRedManager();
                        motorBForward(80);
                        motorABackward(80);
                        if (LeftInfraRedValue == HIGH) {
                          currentDirection = Left;
                          break;
                        }
                      } while (LeftInfraRedValue == LOW && RightInfraRedValue == LOW);
                    }
                    break;
                  }
                case Right:
                  {
                    avoidObject();
                    motorAForward(baseSpeed + turnSpeed);  // Increase speed of left motor
                    motorBBackward(150);                   // Reduce speed of right motor
                    currentDirection = Straight;
                    break;
                  }
                case Left:
                  {
                    avoidObject();
                    motorABackward(150);                   // Reduce speed of left motor
                    motorBForward(baseSpeed + turnSpeed);  // Increase speed of right motor
                    currentDirection = Straight;
                    break;
                  }
                default:
                  {
                    currentDirection = Straight;  // Default to going backward if no valid direction
                    break;
                  }
              }
              break;  // Added break for the Moving case
            }


          case Stopped:
            {
              stop();
              delay(2000);

              evaluatingColour();
              evaluatingColour();
              evaluatingColour();

              if (redFlag) {
                colourIdentified = Red;
              } else if (blueFlag) {
                colourIdentified = Blue;
              } else {
                colourIdentified = Unknown;
              }

              switch (colourIdentified) {
                case Red:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    overtake();
                    delay(10);

                    currentState = Moving;
                    break;  // Added break for the Red case
                  }
                case Blue:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    parkCar();
                    delay(10);

                    currentState = Idle;
                    break;  // Added break for the Blue case
                  }
                case Unknown:
                  {
                    backward(baseSpeed);
                    delay(500);

                    stop();
                    delay(10);

                    turn180();
                    delay(10);

                    currentState = Moving;
                    break;  // Added break for the Unknown case
                  }
                default:
                  {
                    currentState = Moving;
                    break;
                  }
              }
              break;  // Added break for the Stopped case
            }


          case Idle:
            {

              do {
                stop();
                delay(10);

                InfraRedManager();

                distanceFromObject = measureDistance();
                if (distanceFromObject >= 10 && distanceFromObject < 200) {
                  turnRight45();
                  break;
                }
              } while (LeftInfraRedValue == LOW && RightInfraRedValue == LOW);
              do {
                forward(100);
              } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);

              backward(baseSpeed);
              delay(50);

              motorBForward(255);
              motorABackward(255);
              delay(500);  // Adjust the delay to achieve a 240-degree turn

              do {
                InfraRedManager();
                motorBForward(80);
                motorABackward(80);
                if (LeftInfraRedValue == HIGH) {
                  currentState = Moving;
                  currentDirection = Left;
                  break;
                }
              } while (LeftInfraRedValue == LOW && RightInfraRedValue == LOW);
              break;
            }
          default:
            {
              // Handle any unexpected states for currentState
              break;
            }
        }
        break;  // Added break for the WhiteLine case
      }
    default:
      {

        break;
      }
  }
}
