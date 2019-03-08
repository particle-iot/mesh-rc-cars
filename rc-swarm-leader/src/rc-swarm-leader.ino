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
#define MAX_VAL 200

#define WIDE_TURN_DELAY 1650
#define NINETY_DEG_TURN_DELAY 585
#define ONE_THIRTY_FIVE_DEG_TURN_DELAY 878
#define SPIN_DELAY 400

// Sentry mode
#define SENTRY_MODE_DELAY 700
#define SENTRY_MODE_SPEED 100
bool sentryModeEnabled = false;

// Splinter mode
#define SPLINTER_MODE_SPEED 255
#define SPLINTER_MODE_DELAY 5000

// Push mode
#define PUSH_MODE_DELAY 1000

// Orbin Mode
#define ORBIT_VAL 100

String version = "v1.4";
int32_t mode = RC_MODE;
int32_t overrideDelay = 0;

/* MAIN Firmware Functions */

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
  Particle.function("stopDemos", stopDemos);
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

/* MODE CONTROL */

int swarmDemo(String args)
{
  if (mode == RC_MODE)
    return 0;

  String *argVals = getArgs(args);

  // Can be run with adjustable delays to test without recompiling
  overrideDelay = argVals[1].toInt();

  if (argVals[0] == "follow")
  {
    // Run follow the leader demo
    followTheLeader();
  }
  else if (argVals[0] == "spin")
  {
    spinCars();
  }
  else if (argVals[0] == "sentry")
  {
    sentry();
  }
  else if (argVals[0] == "splinter")
  {
    splinter();
  }
  else if (argVals[0] == "push")
  {
    followAndPush();
  }
  else if (argVals[0] == "orbit")
  {
    orbit();
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

      pinMode(leftReverse, INPUT);
      pinMode(leftForward, INPUT);
      pinMode(rightForward, INPUT);
      pinMode(rightReverse, INPUT);
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

int stopDemos(String args)
{
  sentryModeEnabled = false;
}

/* SEQUENCES */

void followTheLeader()
{
  //Move forward,
  moveForward(DRIVE_VAL, 800, 100);

  //Move back to start
  moveBack(DRIVE_VAL, 800, 1000);

  turnRight90(500);
  turnLeft90(10);
}

void spinCars()
{
  spinLeft360();
  motorsOff(200);

  spinRight360();
  motorsOff(200);
}

void sentry()
{
  for (size_t i = 0; i < 4; i++)
  {
    // move forward 1 sec
    moveForward(SENTRY_MODE_SPEED, SENTRY_MODE_DELAY, 100);

    // turn left
    turnLeft90(100);
  } // repeat
}

void splinter()
{
  // Turn 135 Left
  turnLeft135(200);
  // Drive forward for 2 seconds
  moveForward(SPLINTER_MODE_SPEED, SPLINTER_MODE_DELAY, 200);
  // Turn 135 right
  turnRight135(200);
  // Drive forward for 2 sec
  moveForward(SPLINTER_MODE_SPEED, SPLINTER_MODE_DELAY, 200);
}

void followAndPush()
{
  // follow the leader for five sec and stop
  moveForward(DRIVE_VAL, 5000, 200);

  // leader car moves forward for one sec
  analogWrite(leftForward, DRIVE_VAL);
  analogWrite(rightForward, DRIVE_VAL);

  delay(5000);

  // leader car turns 180
  turnLeft90(0);
  turnLeft90(200);

  // leader car moves back one sec
  analogWrite(leftForward, DRIVE_VAL);
  analogWrite(rightForward, DRIVE_VAL);

  delay(5000);

  // leader car continues to move forward, follower cars move backwards
  Mesh.publish("leftR", String(DRIVE_VAL));
  Mesh.publish("rightR", String(DRIVE_VAL));

  delay(5000);

  // Stop all cars
  motorsOff(200);
}

void orbit()
{
  // leader car spins 360 slowly over and over
  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  // follower cars orbit around the leader
  Mesh.publish("leftF", String(ORBIT_VAL));

  delay(5000);

  motorsOff(20);
}

/* PRIMITIVES */

void moveForward(int32_t val, int32_t del, int32_t offDel)
{
  Mesh.publish("leftF", String(val));
  Mesh.publish("rightF", String(val));

  analogWrite(leftForward, val);
  analogWrite(rightForward, val);

  delay(del);

  motorsOff(offDel);
}

void moveBack(int32_t val, int32_t del, int32_t offDel)
{
  Mesh.publish("leftR", String(val));
  Mesh.publish("rightR", String(val));

  analogWrite(leftReverse, val);
  analogWrite(rightReverse, val);

  delay(del);

  motorsOff(offDel);
}

void turnLeft90(int32_t offDel)
{
  Mesh.publish("leftR", String(255));
  Mesh.publish("rightF", String(255));

  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  delay(overrideDelay ? overrideDelay : NINETY_DEG_TURN_DELAY);

  motorsOff(offDel);
}

void turnRight90(int32_t offDel)
{
  Mesh.publish("rightR", String(255));
  Mesh.publish("leftF", String(255));

  analogWrite(rightReverse, 255);
  analogWrite(leftForward, 255);

  delay(overrideDelay ? overrideDelay : NINETY_DEG_TURN_DELAY);

  motorsOff(offDel);
}

void turnLeft135(int32_t offDel)
{
  Mesh.publish("leftR", String(255));
  Mesh.publish("rightF", String(255));

  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  delay(overrideDelay ? overrideDelay : ONE_THIRTY_FIVE_DEG_TURN_DELAY);

  motorsOff(offDel);
}

void turnRight135(int32_t offDel)
{
  Mesh.publish("rightR", String(255));
  Mesh.publish("leftF", String(255));

  analogWrite(rightReverse, 255);
  analogWrite(leftForward, 255);

  delay(overrideDelay ? overrideDelay : ONE_THIRTY_FIVE_DEG_TURN_DELAY);

  motorsOff(offDel);
}

void spinLeft360()
{
  moveForward(255, 400, 200);

  Mesh.publish("leftR", String(255));
  Mesh.publish("rightF", String(255));

  analogWrite(leftReverse, 255);
  analogWrite(rightForward, 255);

  delay(overrideDelay ? overrideDelay : SPIN_DELAY);
}

void spinRight360()
{
  moveForward(255, 400, 200);

  Mesh.publish("leftF", String(255));
  Mesh.publish("rightR", String(255));

  analogWrite(leftForward, 255);
  analogWrite(rightReverse, 255);

  delay(overrideDelay ? overrideDelay : SPIN_DELAY);
}

void motorsOff(int32_t del)
{
  Mesh.publish("allOff", NULL);
  allOff();

  delay(del);
}

void allOff()
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightReverse, 0);
  analogWrite(rightForward, 0);
}

/* UTILITIES */

// Check and control when in RC mode where leader is controlling
// followers via th RC remote.
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