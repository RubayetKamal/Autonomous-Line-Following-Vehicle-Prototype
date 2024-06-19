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

bool whiteLine = true;

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

int RightInfraRedValue = 1;
int LeftInfraRedValue = 1;

void evaluatingColour() {
  Serial.println("------------------------------");
  {


    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);

    float(redEdgeTime) = pulseIn(sensorOut, LOW);
    ;
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
  Serial.println("------------------------------");
  delay(1000);
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
  if (whiteLine) {
    do {
      motorABackward(baseSpeed);
      motorBForward(baseSpeed);
      RightInfraRedValue = digitalRead(RightInfraRed);
      LeftInfraRedValue = digitalRead(LeftInfraRed);

    } while (RightInfraRedValue == HIGH && LeftInfraRedValue == HIGH);
  } else {
    do {
      motorABackward(baseSpeed);
      motorBForward(baseSpeed);
      RightInfraRedValue = digitalRead(RightInfraRed);
      LeftInfraRedValue = digitalRead(LeftInfraRed);

    } while (RightInfraRedValue == LOW && LeftInfraRedValue == LOW);
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
  if (whiteLine) {
    do {
      forward(baseSpeed);
      RightInfraRedValue = digitalRead(RightInfraRed);
      LeftInfraRedValue = digitalRead(LeftInfraRed);
    } while (RightInfraRedValue == HIGH && LeftInfraRedValue == HIGH);
  } else {
    do {
      forward(baseSpeed);
      RightInfraRedValue = digitalRead(RightInfraRed);
      LeftInfraRedValue = digitalRead(LeftInfraRed);
    } while (RightInfraRedValue == LOW && LeftInfraRedValue == LOW);
  }
  stop();
  delay(500);
}


void avoidObject() {
  distanceFromObject = measureDistance();
  Serial.println(distanceFromObject);

  if (distanceFromObject >= 10. && distanceFromObject <= 55.) {
    stop();
    delay(2000);

    evaluatingColour();
    evaluatingColour();
    evaluatingColour();
    Serial.println(redFlag);
    Serial.println(blueFlag);

    if (redFlag) {
      backward(baseSpeed);
      delay(500);

      stop();
      delay(10);

      overtake();
      delay(10);
    } else if (blueFlag) {
      backward(baseSpeed);
      delay(500);

      stop();
      delay(10);

      parkCar();
      delay(10);

    } else {
      backward(baseSpeed);
      delay(500);

      stop();
      delay(10);

      turn180();
      delay(10);
    }
  }
  redFlag = false;
  blueFlag = false;
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


  stop();
  delay(10);  // Small delay after stopping

  // Go back a little bit
  motorABackward(200);
  motorBBackward(220);
  delay(500);  // Adjust the delay to move back a little bit
  if (whiteLine) {
    do {
      stop();
      delay(10);

      distanceFromObject = measureDistance();
      if (distanceFromObject >= 10 && distanceFromObject < 200) {
        turnRight45();
        break;
      }
    } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);
  } else {
    do {
      stop();
      delay(10);

      distanceFromObject = measureDistance();
      if (distanceFromObject >= 10 && distanceFromObject < 200) {
        turnRight45();
        break;
      }
    } while (LeftInfraRedValue == LOW && RightInfraRedValue == LOW);
  }
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
  InfraRedManager();
  avoidObject();
  if (whiteLine) {
    if (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH) {
      forward(baseSpeed);                                                 // Black line detected, go straight
    } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  // Left sensor off the line, right sensor on the line, turn left
      motorABackward(150);                                                //reduce speed of left motor
      motorBForward(baseSpeed + turnSpeed);                               //increase speed of right motor
    } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  //// Right sensor off the line, left sensor on the line, turn right
      motorAForward(baseSpeed + turnSpeed);                               //increase speed of left motor
      motorBBackward(150);                                                //reduce speed of right motor
    } else {                                                              //error meaning both sensors are not finding a black line, meaning car is off tracks
      backward(baseSpeed);

      motorBForward(255);
      motorABackward(255);
      delay(500);  // Adjust the delay to achieve a 90-degree turn

      do {
        if (LeftInfraRedValue == LOW) {
          motorABackward(150);                   //reduce speed of left motor
          motorBForward(baseSpeed + turnSpeed);  //increase speed of right motor
          break;
        }
      } while (LeftInfraRedValue == HIGH && RightInfraRedValue == HIGH);
    }



    //////For Black Line:



  } else {
    if (LeftInfraRedValue == LOW && RightInfraRedValue == LOW) {
      forward(baseSpeed);                                                 // Black line detected, go straight
    } else if (LeftInfraRedValue == HIGH && RightInfraRedValue == LOW) {  // Left sensor off the line, right sensor on the line, turn left
      motorABackward(150);                                                //reduce speed of left motor
      motorBForward(baseSpeed + turnSpeed);                               //increase speed of right motor
    } else if (LeftInfraRedValue == LOW && RightInfraRedValue == HIGH) {  //// Right sensor off the line, left sensor on the line, turn right
      motorAForward(baseSpeed + turnSpeed);                               //increase speed of left motor
      motorBBackward(150);                                                //reduce speed of right motor
    } else {                                                              //error meaning both sensors are not finding a black line, meaning car is off tracks
      backward(baseSpeed);

      motorBForward(255);
      motorABackward(255);
      delay(500);  // Adjust the delay to achieve a 270-degree turn

      do {
        if (LeftInfraRedValue == HIGH) {
          motorABackward(150);                   //reduce speed of left motor
          motorBForward(baseSpeed + turnSpeed);  //increase speed of right motor
          break;
        }
      } while (LeftInfraRedValue == LOW && RightInfraRedValue == LOW);
    }
  }
}
