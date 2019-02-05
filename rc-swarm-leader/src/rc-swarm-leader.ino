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
#define DRIVE_VAL 125
#define SENTRY_VAL 90
#define MAX_VAL 200

#define WIDE_TURN_DELAY 1650
#define TIGHT_TURN_DELAY 450

String version = "v1.1";
int32_t mode = RC_MODE;
int32_t overrideDelay = 0;

void setup()
{
  Serial.begin(9600);

  // Need to switch this depending on whether we are in RC or Demo Mode
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

  String *argVals = getArgs(args);

  if (argVals[0] == "follow")
  {
    // Run follow the leader demo
    // Can be run with adjustable delays to test without recompiling
    overrideDelay = argVals[1].toInt();
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

      pinMode(leftReverse, OUTPUT);
      pinMode(leftForward, OUTPUT);
      pinMode(rightForward, OUTPUT);
      pinMode(rightReverse, OUTPUT);
    }
    else if (args == "demo")
    {
      mode = DEMO_MODE;

      pinMode(leftReverse, OUTPUT);
      pinMode(leftForward, OUTPUT);
      pinMode(rightForward, OUTPUT);
      pinMode(rightReverse, OUTPUT);
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
  //Move forward, and accelerate at the end
  moveForward(DRIVE_VAL, 800);
  motorsOff(1000);

  //Move back to start
  moveBack(DRIVE_VAL, 800);
  motorsOff(1000);

  turnRight90();
  motorsOff(500);

  turnLeft90();
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

  analogWrite(leftForward, val);
  analogWrite(rightForward, val);

  delay(del);
}

void moveBack(int32_t val, int32_t del)
{
  Mesh.publish("leftR", String(val));
  Mesh.publish("rightR", String(val));

  analogWrite(leftReverse, val);
  analogWrite(rightReverse, val);

  delay(del);
}

void turnRight90()
{
  Mesh.publish("leftR", String(255));
  Mesh.publish("rightF", String(255));

  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  delay(overrideDelay ? overrideDelay : TIGHT_TURN_DELAY);
}

void turnLeft90()
{
  Mesh.publish("rightR", String(255));
  Mesh.publish("leftF", String(255));

  analogWrite(rightReverse, 255);
  analogWrite(leftForward, 255);

  delay(overrideDelay ? overrideDelay : TIGHT_TURN_DELAY);
}

void allOff()
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightReverse, 0);
  analogWrite(rightForward, 0);
}

// Utility function to split a Particle function args string into component parts
String *getArgs(String args)
{
  String *vals = new String[2];
  int separatorIndex = args.indexOf(",");

  if (separatorIndex == -1)
  {
    vals[0] = args;
  }
  else
  {
    vals[0] = args.substring(0, separatorIndex);
    vals[1] = args.substring(separatorIndex + 1);
  }

  return vals;
}