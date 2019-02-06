#include "application.h"
#line 1 "/Users/bsatrom/Development/particle/rc-mesh/rc-mesh-vehicle/src/rc-mesh-vehicle.ino"
/*
 * Project rc-mesh-vehicle
 * Description: It's a Xenon-powered RC Car!
 * Author: Brandon Satrom <brandon@particle.io>
 * Date: 01/10/2018
 */

/* Connections to Thunder Tumbler PCB 
* Xenon | Tumbler
* ---------------
*  A0   | 9 (Left Wheel Back)
*  A1   | 12 (Left Wheel Forward)
*  A2   | 13 (Right Wheel Forward)
*  A3   | 16 (Right Wheel Back)
* GND   | 10,11,14,15 (GND)
*/

// Wheel pin mappings
void setup();
void stopDemo(const char *event, const char *data);
void runDemo(const char *event, const char *data);
void basicDemo();
void sentryDemo();
void spinDemo();
void allOff();
void goForward(int speed);
void goBack(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void spinLeft360();
void spinRight360();
void loop();
#line 19 "/Users/bsatrom/Development/particle/rc-mesh/rc-mesh-vehicle/src/rc-mesh-vehicle.ino"
int leftReverse = A0;
int leftForward = A1;
int rightForward = A2;
int rightReverse = A3;

// Speed and delay variables
#define SPIN_DELAY 400

int speed = 85;
int turnSpeed = 255;
int forwardDelay = 1000;
int backDelay = 1000;
int turnDelay = 2000;
String version = "v1.2";

void setup()
{
  pinMode(leftReverse, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);

  digitalWrite(leftReverse, LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, LOW);

  Particle.publish("rc-car-online", version);
  Particle.variable("fw-version", version);

  Mesh.subscribe("run-demo", runDemo);
}

void stopDemo(const char *event, const char *data)
{
  allOff();
}

void runDemo(const char *event, const char *data)
{
  if (strcmp(data, "basic") == true)
  {
    // Run follow the leader demo
    basicDemo();
  }
  else if (strcmp(data, "spin") == true)
  {
    spinDemo();
  }
  else if (strcmp(data, "sentry") == true)
  {
    sentryDemo();
  }
}

void basicDemo()
{
  allOff();

  goForward(speed);
  delay(forwardDelay);

  goBack(speed);
  delay(backDelay);

  // Max spin to raise up on back tires
  turnLeft(turnSpeed);
  delay(turnDelay);

  allOff();
}

void sentryDemo()
{
}

void spinDemo()
{
  spinLeft360();
  allOff();

  spinRight360();
  allOff();
}

void allOff()
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 0);

  delay(50);
}

void goForward(int speed)
{
  allOff();

  analogWrite(rightForward, speed);
  analogWrite(leftForward, speed);
}

void goBack(int speed)
{
  allOff();

  analogWrite(rightReverse, speed);
  analogWrite(leftReverse, speed);
}

void turnLeft(int speed)
{
  allOff();

  analogWrite(rightForward, speed);
}

void turnRight(int speed)
{
  allOff();

  analogWrite(leftForward, speed);
}

void spinLeft360()
{
  goForward(255);
  delay(400);
  allOff();

  delay(150);

  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  delay(SPIN_DELAY);
}

void spinRight360()
{
  goForward(255);
  delay(400);
  allOff();

  delay(150);

  analogWrite(leftForward, 255);
  analogWrite(rightReverse, 255);

  delay(SPIN_DELAY);
}

void loop()
{
  // Nothing needed here. Just waiting on those Mesh commands!
}