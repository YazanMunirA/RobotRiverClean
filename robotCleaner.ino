// ================= PIN CONFIG =================

// Ultrasonic Sensors
#define TRIG_FRONT 2
#define ECHO_FRONT 3

#define TRIG_LEFT 4
#define ECHO_LEFT 7

#define TRIG_RIGHT 8
#define ECHO_RIGHT 11

// Button
#define START_BTN 12

// Drive motors (PWM)
#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

// Belt motors
#define BELT1 A1
#define BELT1 A0
#define BELT2 A3
#define BELT2 A2



#define MAX_DISTANCE 150   // cm
#define TARGET_DISTANCE 60 // cm threshold
#define MOTOR_SPEED 180    // PWM (0-255)
#define TURN_SPEED 150


bool systemStarted = false;

long readDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000); 

  if (duration == 0) return MAX_DISTANCE;

  long distance = duration * 0.034 / 2;

  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

  return distance;
}


void moveForward(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, speed);
  analogWrite(IN4, 0);
}

void turnLeft(int speed) {
  analogWrite(IN1, 0);
  analogWrite(IN2, speed);
  analogWrite(IN3, speed);
  analogWrite(IN4, 0);
}

void turnRight(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, speed);
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Belt motors
void beltON() {
  digitalWrite(BELT1, HIGH);
  digitalWrite(BELT2, HIGH);
}

void beltOFF() {
  digitalWrite(BELT1, LOW);
  digitalWrite(BELT2, LOW);
}


void setup() {
  Serial.begin(9600);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);

  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  pinMode(START_BTN, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BELT1, OUTPUT);
  pinMode(BELT2, OUTPUT);

  stopMotors();
  beltOFF();
}


void loop() {

  // Start button
  if (digitalRead(START_BTN) == LOW) {
    systemStarted = true;
    delay(300);
  }

  if (!systemStarted) return;

 
  long front = readDistance(TRIG_FRONT, ECHO_FRONT);
  long left  = readDistance(TRIG_LEFT, ECHO_LEFT);
  long right = readDistance(TRIG_RIGHT, ECHO_RIGHT);

  // Debug
  Serial.print("F: "); Serial.print(front);
  Serial.print(" L: "); Serial.print(left);
  Serial.print(" R: "); Serial.println(right);

  

  // Case 1: Object in front
  if (front < TARGET_DISTANCE) {
    moveForward(MOTOR_SPEED);
    beltON();
  }


  else if (left < right && left < TARGET_DISTANCE) {
    turnLeft(TURN_SPEED);
    beltOFF();
  }


  else if (right < left && right < TARGET_DISTANCE) {
    turnRight(TURN_SPEED);
    beltOFF();
  }


  else {

    turnLeft(120);
    beltOFF();
  }

  delay(50); 
}