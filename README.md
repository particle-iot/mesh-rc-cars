# rc-mesh

🚗💨 🚗🚗🚗

Firmware for mesh-networked RC cars. Includes standalone projects for the following behaviors:

### 1. ✅ Demo Mode - runs a basic demo sequence triggered from a gateway device.

- `rc-mesh-controller` - Gateway controller firmware (Surfaces a `Particle.function` that can be called via the console or mobile device).
- `rc-mesh-vehicle` - End device firmware that listens for a mesh event (via `Mesh.subscribe`) and triggers a pre-defined sequence of movements.

### 2. ✅ Swarm with RC-powered Leader - Uses the remote control to control a leader car; leader car sends instructions to followers via Mesh events.

- `rc-swarm-leader` - Reads voltages sent to the car via the remote control and uses `Mesh.publish` to send those to all listeners.
- `rc-swarm-follower` - Listens for leader messages via `Mesh.subscribe` and sets the voltage on a corresponding pin.

### 3. ️✅ Swarm sequences

- Modification of #2 with pre-arranged sequences.
- `rc-swarm-follower` - Same functionality as #2: continue to respond to Mesh messages to move each motor/wheel.
- `rc-swarm-leader` - Modified from #2 to include
  - Swarm modes - RC Mode (default), Demo Mode
  - Function to switch swarm modes
  - Function to trigger swarm demos
    - Follow the leader - basic forward and back demo with ending spin
    - ️Splinter - cars separate in three different directions and come back together
    - Follow the leader and push - Leader car tells followers to stop, goes forward 2 seconds, turns around, goes back 2 seconds, then tells followers to move backwards as it keeps moving forward.
    - Sentry mode - Square with right-angle turns
    - Orbit - follower cars orbit a stationary leader car
  - Demo "primitives" that encapsulate car movements
    - Basic (`forward`, `back`, `circle`)
    - Turns (`turnRight45`, `turnRight90`, etc.)
    - Spins (`spin360Left`, `spin360Right`)

## A few ideas for how this project could be extended

*AKA send us a PR!*

### 4. Swarm sequence with collision-detection

- Leader fitted with ultrasonic distance sensor. Moves forward until it sees an obstacle, then moves back and turns around.
- Emits mesh commands to followers to do the same

### 5. Swarm-based mesh range testing

- All networked vehicles drive away from the gateway until their network signal strength falls below a threshold.
- `Gather` command summons all vehicles back until the network signal goes above a threshold.

### 6. Swarm sequence "mapping" with RC remote

- Modification of #2 that logs each swarm movement from the remote control.
- Can be used to program sequences (manually or automatically)

## Instructions for building and flashing the firmware

*You should already have a mesh network with at least one Gateway (Argon or Boron) and one or more Xenons for this project. The Gateway can be attached to a car in the swarm, or separate with Xenons on every RC car in the swarm.*

1. Clone this repo
2. Open the `rc-mesh.code-workspace` in VS Code (If you don't have the Particle Workbench extension for VS Code, [install it!](https://marketplace.visualstudio.com/items?itemName=particle.particle-vscode-pack))
3. For local compilation and flashing, install a local compiler for 3rd generation devices (Xenon, Argon, Boron) using the "Particle: Install Local Compiler" command in the VS Code command palette. 
4. To flash a swarm leader, run the "Particle: Configure Workspace for Device" command and follow the prompts, selecting the `rc-sawarm-leader` project, then run either the "Particle: Cloud Flash" or "Particle: Flash application (local)" command.
5. Repeat this project for each swarm follower, choosing the swarm `rc-swarm-follower` project".
6. Once all the devices are flashed, run the demos using functions in the [Particle Console](https://console.particle.io)