// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
#include <AFMotor.h>

int replaystage;


#define leapTime 200

//defining pins and variables
#define lefts A2
#define rights A5
#define midleft A3
#define midright A4

#define led 13 

//defining motors
AF_DCMotor motor1(1, MOTOR12_8KHZ);
AF_DCMotor motor2(2, MOTOR12_8KHZ);
/*
  AF_DCMotor motor1(3, MOTOR12_8KHZ);
  AF_DCMotor motor2(4, MOTOR12_8KHZ);
*/
char path[30] = {};
int pathLength;
int readLength;

void setup() {
  //setting the speed of motors
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  //declaring pin types
  pinMode(lefts, INPUT);
  pinMode(rights, INPUT);
  pinMode(midleft, INPUT);
  pinMode(midright, INPUT);
  //begin serial communication
  Serial.begin(9600);

}

void loop() {
  //printing values of the sensors to the serial monitor
  readSensors();

  if (analogRead(lefts) <= 400 && analogRead(rights) <= 400 &&
      (!analogRead(midleft) <= 400 || !analogRead(midright) <= 400)) {
    straight();
  }
  else
  {
    leftHandWall();
  }
}




void readSensors()
{
  Serial.println(analogRead(lefts));
  Serial.println(analogRead(rights));
  Serial.println(analogRead(midleft));
  Serial.println(analogRead(midright));
}




void leftHandWall()
{
  if (!analogRead(lefts) <= 400 && !analogRead(rights) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(leapTime);
    readSensors();


    if (!analogRead(lefts) <= 400 || !analogRead(rights) <= 400)
    {
      done();
    }

    if (analogRead(lefts) <= 400 && analogRead(rights) <= 400)
    {
      turnLeft();
    }

  }

  if (!analogRead(lefts) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(leapTime);
    readSensors();



    if (analogRead(lefts) <= 400 && analogRead(rights) <= 400)
    {
      turnLeft();
    }
    else {
      done();
    }
  }


  if (!analogRead(rights) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(30);
    readSensors();


    if (!analogRead(lefts) <= 400)
    {
      delay(leapTime - 30);
      readSensors();


      if (!analogRead(rights) <= 400 && !analogRead(lefts) <= 400) {

        done();
      }

      else {
        turnLeft();
        return;
      }
    }

    delay(leapTime - 30);
    readSensors();

    if (analogRead(lefts) <= 400 && analogRead(midleft) <= 400 &&
        analogRead(midright) <= 400 && analogRead(rights) <= 400) {
      turnRight();
      return;
    }

    path[pathLength] = 'S';
    pathLength++;
    if (path[pathLength - 2] == 'B') {

      shortPath();
    }
    straight();
  }
  readSensors();

  if (analogRead(lefts) <= 400 && analogRead(midleft) <= 400 &&
      analogRead(midright) <= 400 && analogRead(rights) <= 400) {
    turnAround();
  }
}

void done()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);

  replaystage = 1;
  path[pathLength] = 'D';
  pathLength++;

  delay(500);
  replay();
}

void turnLeft() {
  while (!analogRead(midright) <= 400 && !analogRead(midleft) <= 400)
  {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(10);
  }
  while (!analogRead(midright) <= 400)
  {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(10);
  }

  if (replaystage == 0) {
    path[pathLength] = 'L';

    pathLength++;
    if (path[pathLength - 2] == 'B') {

      shortPath();
    }
  }
}


void turnRight() {
  while (!analogRead(midright) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(10);
  }
  while (analogRead(midright) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(10);
  }

  while (analogRead(midleft) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(10);
  }

  if (replaystage == 0) {
    path[pathLength] = 'R';
    Serial.println("r");
    pathLength++;
    Serial.print("Path length: ");
    Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      Serial.println("shortening path");
      shortPath();
    }
  }
}


void straight()
{
  if (analogRead(midleft) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(1);
    motor1.run(FORWARD);
    motor2.run(RELEASE);
    delay(2);
    return;
  }
  if (analogRead(midright) <= 400)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(1);
    motor1.run(RELEASE);
    motor2.run(FORWARD);
    delay(2);
    return;
  }

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(2);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(1);
}

void turnAround() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(150);
  while (analogRead(midleft) <= 400) {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(2);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(1);

  }
  path[pathLength] = 'B';
  pathLength++;
  straight();
}

void shortPath() {
  int shortDone = 0;
  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R') {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test1");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'S' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test2");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test3");
    shortDone = 1;
  }
  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test4");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test5");
    shortDone = 1;
  }
  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'S';
    //Serial.println("test6");
    shortDone = 1;
  }
  path[pathLength + 1] = 'D';
  path[pathLength + 2] = 'D';
  pathLength++;
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
}


void printPath() {
  Serial.println("+++++++++++++++++");
  int x;
  while (x <= pathLength) {
    Serial.println(path[x]);
    x++;
  }
  Serial.println("+++++++++++++++++");
}


void replay() {
  readSensors();
  if (analogRead(lefts) <= 400 && analogRead(rights) <= 400) {
    straight();
  }
  else {
    if (path[readLength] == 'D') {

      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(100);

      motor1.run(RELEASE);
      motor2.run(RELEASE);
     // endMotion();
    }
    if (path[readLength] == 'L') {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(leapTime);
      turnLeft();
    }
    if (path[readLength] == 'R') {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(leapTime);
      turnRight();
    }
    if (path[readLength] == 'S') {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(leapTime);
      straight();
    }
    readLength++;
  }

  replay();

}

void endMotion() {
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);
  digitalWrite(led, HIGH);
  delay(500);
  endMotion();
}
