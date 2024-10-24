const int trigPin = A1;
const int echoPin = A2;

const int MIN_DISTANCE = 10;
const int MAX_DISTANCE = 25;


int frontDist;
int rightDist;
int rightDiagDist;
int backDist;

int left_sensor, right_sensor;


void setup() {
  Serial.begin(9600);

  //establish motor direction toggle pins

  pinMode(12, OUTPUT); //motor A -- HIGH = forward, LOW = backwards
  pinMode(13, OUTPUT); //motor B -- HIGH = forwards, LOW = backwards
  
  //establish motor brake pins

  pinMode(9, OUTPUT); //brake motor A
  pinMode(8, OUTPUT); //brake motor B

  //sensor array pins

  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {
  
  selectPin(4);
  frontDist = getDistance();
  delay(100);

  selectPin(5);
  rightDiagDist = getDistance();
  delay(100);
 
  selectPin(6);
  rightDist = getDistance();
  delay(100);
  
// Prints the distance on the Serial Monitor

  Serial.print("Front: ");
  Serial.println(frontDist);
  Serial.print("DiagRight: ");
  Serial.println(rightDiagDist);
  Serial.print("Right: ");
  Serial.println(rightDist);

  if(frontDist <= MIN_DISTANCE || rightDiagDist <= MIN_DISTANCE/2){
    moveLeft();
    delay(200);
    follow_wall();
  }
  else {
    follow_line();
  }



//----------------------------- METHODS --------------------------------

//------ LINE FOLLOWING -------

void follow_line(int right_sensor, int left_sensor) {
  if (left_sensor == 1 && right_sensor == 1) { moveForward();}

  else if(right_sensor == 0 && left_sensor == 1) { moveLeft(); }

  else if (right_sensor == 1 && left_sensor == 0) { moveRight(); }

  else { moveForward(); }
}


//----- RECURSIVE FUNCTION FOR WALL FOLLOWING ------

void follow_wall() {

  selectPin(6);   // right sensor
  rightDist = getDistance();

  selectPin(4);   // front sensor
  frontDist = getDistance();
  delay(100);

  
  while (left_sensor == 0 || right_sensor == 0) {

    while (rightDist <= MIN_DISTANCE) {

      if (frontDist <= MIN_DISTANCE || rightDiagDist <= MIN_DISTANCE/2) {
        moveLeft();
        delay(400);
        }

      else {
        moveForward();
        delay(500);
      }

      selectPin(6);   // right sensor
      rightDist = getDistance();
      delay(100)

      selectPin(4);   // front sensor
      frontDist = getDistance();
      delay(100);

      selectPin(5);   // diagonal right sensor
      rightDiagDist = getDistance();
      delay(100);
    }
    
    // Continue with wall following until the robot is on the line again
    moveRight();
    delay(200);

    moveForward();
    delay(400);

    follow_wall();

  }
  
  // Move back to loop when line is detectd by sensors
  moveLeft();
  delay(200);
}


//----- GET DISTANCE FROM ULTRASONIC SENSORS -----

int getDistance() {
  long duration;
  
  delay(10);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  int dstc = (duration/2) / 29.1;

  return dstc;
}

//----- MOVE FORWARD -----
void moveForward() {
   //Motor A forward @ full speed
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  digitalWrite(3, HIGH);   //Spins the motor on Channel A at full speed

  //Motor B backward @ full speed
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  digitalWrite(11, HIGH);    //Spins the motor on Channel B at full speed
}

//----- MOVE RIGHT -----
void moveRight() {
    //Motor A forward @ full speed
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 175);   //Spins the motor on Channel A at 175

  //Motor B backward @ full speed
  digitalWrite(13, LOW);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 25);    //Spins the motor on Channel B at 25
}

//----- MOVE LEFT -----
void moveLeft() {
    //Motor A forward @ full speed
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 25);   //Spins the motor on Channel A at 25

  //Motor B backward @ full speed
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 175);    //Spins the motor on Channel B at 175
}

//----- SELECT SENSOR FROM ARRAY -----
void selectPin(int sensor) {

  switch(sensor) {
    case 1: 
      digitalWrite(A3, LOW);
      digitalWrite(A4, LOW);
      digitalWrite(A5, LOW);
      break;
    case 2: 
      digitalWrite(A3, HIGH);
      digitalWrite(A4, LOW);
      digitalWrite(A5, LOW);
      break;
    case 3: 
      digitalWrite(A3, LOW);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, LOW);
      break;
    case 4: 
      digitalWrite(A3, HIGH);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, LOW);
      break;
    case 5: 
      digitalWrite(A3, LOW);
      digitalWrite(A4, LOW);
      digitalWrite(A5, HIGH);
      break;
    case 6: 
      digitalWrite(A3, HIGH);
      digitalWrite(A4, LOW);
      digitalWrite(A5, HIGH);
      break;
    case 7: 
      digitalWrite(A3, LOW);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, HIGH);
      break;
    case 8: 
      digitalWrite(A3, HIGH);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, HIGH);
      break;
  }
}
