#include "application.h"
#line 1 "/Users/bsatrom/Development/particle/rc-mesh/rc-mesh-controller/src/rc-mesh-controller.ino"
/*
 * Project rc-mesh-controller
 * Description: It's a mesh RC Car controller!
 * Author: Brandon Satrom <brandon@particle.io>
 * Date: 01/17/2018
 */

void setup();
int runDemo (String command);
void loop();
#line 8 "/Users/bsatrom/Development/particle/rc-mesh/rc-mesh-controller/src/rc-mesh-controller.ino"
void setup() {
  Particle.function("runDemo", runDemo);
}

int runDemo (String command)
{
  Mesh.publish("run-demo", NULL);

  return 1;
}

void loop() {
  // Nothing needed here
}