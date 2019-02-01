#include "application.h"
#line 1 "/Users/bsatrom/Development/particle/rc-mesh/rc-swarm-leader/src/rc-swarm-leader.ino"
/*
 * Project rc-swarm-leader
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
void loop();
void checkPin(int pin, int32_t *lastVal, const char *event);
#line 20 "/Users/bsatrom/Development/particle/rc-mesh/rc-swarm-leader/src/rc-swarm-leader.ino"
int leftReverse = A0;
int leftForward = A1;
int rightForward = A2;
int rightReverse = A3;

int32_t lastLeftRVal = 0;
int32_t lastLeftFVal = 0;
int32_t lastRightRVal = 0;
int32_t lastRightFVal = 0;

#define MIN_PIN_VAL 150
#define DRIVE_VAL 200

String version = "v1.0";

void setup()
{
  Serial.begin(9600);

  pinMode(leftReverse, INPUT);
  pinMode(leftForward, INPUT);
  pinMode(rightForward, INPUT);
  pinMode(rightReverse, INPUT);

  Particle.publish("swarm-leader-online", version);
  Particle.variable("fw-version", version);
}

void loop()
{
  checkPin(leftReverse, &lastLeftRVal, "leftR");
  checkPin(leftForward, &lastLeftFVal, "leftF");
  checkPin(rightReverse, &lastRightRVal, "rightR");
  checkPin(rightForward, &lastRightFVal, "rightF");
}

void checkPin(int pin, int32_t *lastVal, const char *event)
{
  int32_t pinVal = analogRead(pin) / 16;
  
  if (pinVal > MIN_PIN_VAL)
    pinVal = DRIVE_VAL;
  else
    pinVal = 0;

  if (pinVal != *lastVal && pinVal == DRIVE_VAL)
  {
    *lastVal = pinVal;

    Mesh.publish(event, String(DRIVE_VAL));
    Serial.printlnf("%s val: %i", event, DRIVE_VAL);
  } else if (pinVal == 0 && *lastVal != 0) {
    *lastVal = 0;
    
    Mesh.publish(event, String(0));
    Serial.printlnf("%s val: %i", event, 0);
  }
}