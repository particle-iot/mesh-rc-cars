#include "application.h"
#line 1 "/Users/bsatrom/Development/particle/rc-mesh/rc-swarm-follower/src/rc-swarm-follower.ino"
/*
 * Project rc-swarm-follower
 * Description: Swarm bot demo with leader RC car controlled by RC remote. Follower cars obey Mesh commands to 
 *   move along with leader car
 * Author: Brandon Satrom <brandon@particle.io>
 * Date: 1/30/2019
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
void leftR(const char *event, const char *data);
void leftF(const char *event, const char *data);
void rightR(const char *event, const char *data);
void rightF(const char *event, const char *data);
void allOff(const char *event, const char *data);
void move(int pin, const char *speed);
void loop();
#line 20 "/Users/bsatrom/Development/particle/rc-mesh/rc-swarm-follower/src/rc-swarm-follower.ino"
int32_t leftReverse = A0;
int32_t leftForward = A1;
int32_t rightForward = A2;
int32_t rightReverse = A3;

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

  Particle.publish("swarm-follower-online", version);
  Particle.variable("fw-version", version);

  Mesh.subscribe("leftR", leftR);
  Mesh.subscribe("leftF", leftF);
  Mesh.subscribe("rightR", rightR);
  Mesh.subscribe("rightF", rightF);
  Mesh.subscribe("allOff", allOff);
}

void leftR(const char *event, const char *data)
{
  move(leftReverse, data);
}

void leftF(const char *event, const char *data)
{
  move(leftForward, data);
}

void rightR(const char *event, const char *data)
{
  move(rightReverse, data);
}

void rightF(const char *event, const char *data)
{
  move(rightForward, data);
}

void allOff(const char *event, const char *data)
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightReverse, 0);
  analogWrite(rightForward, 0);
}

void move(int pin, const char *speed)
{
  int32_t speedVal = atoi(speed);

  if (speedVal > 16)
  {
    analogWrite(pin, speedVal);
  }
  else
  {
    analogWrite(pin, 0);
  }
}

void loop()
{
  // Nothing to see here. Just doing what the leader tells us to do
}