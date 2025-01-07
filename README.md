# stable-c
### Object Stabilization in C
**Current idea:** Create a DIY arduino drone and program the control/stabilization logic in C.

- CLI drone *simulator* shows basic thrust throttling logic controls, use arrow keys.
- Basic stabilization algorithm works, will need tweaking once real data is analyzed.
- Need to experiment with telemetry data from sensors to simulate realistic numbers for a stabilization algorithm.

### Run the code (UNIX)
> gcc stabilization.c && ./stabilization

or

> gcc drone.c && ./drone
