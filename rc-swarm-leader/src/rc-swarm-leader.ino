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
#define DRIVE_VAL 200

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