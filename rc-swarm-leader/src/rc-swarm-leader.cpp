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
int swarmDemo(String args);
int switchSwarmMode(String args);
void checkPin(int pin, int32_t *lastVal, const char *event);
void followTheLeader();
void motorsOff(int32_t del);
void moveForward(int32_t val, int32_t del);
void moveBack(int32_t val, int32_t del);
void turnWideRightReverse();
void turnTightRightReverse();
void allOff();
#line 20 "/Users/bsatrom/Development/particle/rc-mesh/rc-swarm-leader/src/rc-swarm-leader.ino"
int32_t leftReverse = A0;
int32_t leftForward = A1;
int32_t rightForward = A2;
int32_t rightReverse = A3;

int32_t lastLeftRVal = 0;
int32_t lastLeftFVal = 0;
int32_t lastRightRVal = 0;
int32_t lastRightFVal = 0;

const int32_t RC_MODE = 1;
const int32_t DEMO_MODE = 2;

#define MIN_PIN_VAL 150
#define DRIVE_VAL 200
#define MAX_VAL 255

#define WIDE_TURN_DELAY 1650
#define TIGHT_TURN_DELAY 500

String version = "v1.1";
int32_t mode = RC_MODE;

void setup()
{
  Serial.begin(9600);

  pinMode(leftReverse, INPUT);
  pinMode(leftForward, INPUT);
  pinMode(rightForward, INPUT);
  pinMode(rightReverse, INPUT);

  Particle.publish("swarm-leader-online", version);
  Particle.publish("swarm-mode", mode == 1 ? "RC Mode" : "Demo Mode");
  Particle.variable("fw-version", version);
  Particle.variable("swarm-mode", mode);

  Particle.function("switchMode", switchSwarmMode);
  Particle.function("swarmDemo", swarmDemo);
}

void loop()
{
  if (mode == RC_MODE)
  {
    checkPin(leftReverse, &lastLeftRVal, "leftR");
    checkPin(leftForward, &lastLeftFVal, "leftF");
    checkPin(rightReverse, &lastRightRVal, "rightR");
    checkPin(rightForward, &lastRightFVal, "rightF");
  }
}

int swarmDemo(String args)
{
  if (mode == RC_MODE)
    return 0;

  if (args == "follow")
  {
    // Run follow the leader demo
    followTheLeader();
  }

  return 1;
}

int switchSwarmMode(String args)
{
  int32_t newMode = args.toInt();

  if (newMode == 0) // It's a String
  {
    if (args == "rc")
    {
      mode = RC_MODE;
    }
    else if (args == "demo")
    {
      mode = DEMO_MODE;
    }

    Particle.publish("swarm-mode", mode == 1 ? "RC Mode" : "Demo Mode");
  }
  else
  {
    mode = newMode;

    Particle.publish("swarm-mode", mode == 1 ? "RC Mode" : "Demo Mode");
  }
  return 1;
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
  }
  else if (pinVal == 0 && *lastVal != 0)
  {
    *lastVal = 0;

    Mesh.publish(event, String(0));
    Serial.printlnf("%s val: %i", event, 0);
  }
}

void followTheLeader()
{
  // Move forward, and accelerate at the end
  //moveForward(DRIVE_VAL, 600);
  //moveForward(MAX_VAL, 300);
  //motorsOff(1000);

  // move back to start
  //moveBack(DRIVE_VAL, 1200);
  //motorsOff(1000);

  turnWideRightReverse();
  //turnTightRightReverse();

  motorsOff(10);
}

void motorsOff(int32_t del)
{
  Mesh.publish("allOff", NULL);
  allOff();

  delay(del);
}

void moveForward(int32_t val, int32_t del)
{
  Mesh.publish("leftF", String(val));
  Mesh.publish("rightF", String(val));

  delay(del);
}

void moveBack(int32_t val, int32_t del)
{
  Mesh.publish("leftR", String(val));
  Mesh.publish("rightR", String(val));

  delay(del);
}

void turnWideRightReverse()
{
  Mesh.publish("leftR", String(255));

  delay(WIDE_TURN_DELAY);
}

void turnTightRightReverse()
{
  Mesh.publish("leftR", String(255));
  Mesh.publish("rightF", String(255));

  delay(TIGHT_TURN_DELAY);
}

void allOff()
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightReverse, 0);
  analogWrite(rightForward, 0);
}