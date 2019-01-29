/*
 * Project rc-swarm-follower
 * Description:
 * Author:
 * Date:
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
int leftReverse = A0;
int leftForward = A1;
int rightForward = A2;
int rightReverse = A3;

int speed = 85;

String version = "v0.8";

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

  Mesh.subscribe("goForward", goForward);
  Mesh.subscribe("goBack", goBack);
}

void goForward(const char *event, const char *data)
{
  analogWrite(rightForward, speed);
  analogWrite(leftForward, speed);

  delay(1000);

  allOff();
}

void goBack(const char *event, const char *data)
{
  analogWrite(rightReverse, speed);
  analogWrite(leftReverse, speed);

  delay(1000);

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

void loop()
{
}