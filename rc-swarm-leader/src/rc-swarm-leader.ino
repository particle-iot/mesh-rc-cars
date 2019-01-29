/*
 * Project rc-swarm-leader
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
int lastLeftRVal = 0;
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

  Particle.publish("swarm-leader-online", version);
  Particle.variable("fw-version", version);

  Particle.function("forward", goForward);
  Particle.function("back", goBack);
  Particle.function("stop", stop);
}

int stop(String args)
{
  off();

  return 1;
}

void off()
{
  Mesh.publish("allOff", NULL);
}

void allOff()
{
  analogWrite(leftReverse, 0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 0);
}

int goForward(String args)
{
  off();

  Mesh.publish("goForward", NULL);
  forward();

  return 1;
}

void forward()
{
  analogWrite(rightForward, speed);
  analogWrite(leftForward, speed);

  delay(1000);

  allOff();
}

int goBack(String args)
{
  off();

  Mesh.publish("goBack", NULL);
  back();

  return 1;
}

void back()
{
  analogWrite(rightReverse, speed);
  analogWrite(leftReverse, speed);

  delay(1000);

  allOff();
}

void loop()
{
  /*int leftRVal = analogRead(leftReverse);

  if (leftRVal != lastLeftRVal)
  {
    lastLeftRVal = leftRVal;

    Mesh.publish("leftR", String(leftRVal));
    Particle.publish("leftR", String(leftRVal));
  }*/
}