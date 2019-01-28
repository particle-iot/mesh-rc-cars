# rc-mesh

🚗💨 🚗🚗🚗

Firmware for mesh-networked RC cars. Includes standalone projects for the following behaviors:

### 1. ✅ Demo Mode - runs a basic demo sequence triggered from a gateway device.

- `rc-mesh-controller` - Gateway controller firmware (Surfaces a `Particle.function` that can be called via the console or mobile device).
- `rc-mesh-vehicle` - End device firmware that listens for a mesh event (via `Mesh.subscribe`) and triggers a pre-defined sequence of movements.

### 2. [Under development] Swarm with RC-powered Leader

- `rc-swarm-controller`
- `rc-swarm-leader`
- `rc-swarm-follower`

### 3. [Coming soon] Swarm sequences

- Modification of #2 with pre-arranged sequences

### 4. [Coming soon] Swarm sequence with collision-detection

- Leader fitted with ultrasonic distance sensor. Moves forward until it sees an obstacle, then moves back and turns around.
- Emits mesh commands to followers to do the same

### 5. [Coming soon] Swarm-based mesh range testing

- All networked vehicles drive away from the gateway until their network signal strength falls below a threshold.
- `Gather` command summons all vehicles back until the network signal goes above a threshold.

### 6. [Coming soon] Swarm sequence "mapping" with RC remote

- Modification of #2 that logs each swarm movement from the remote control.
- Can be used to program sequences (manually or automatically)
