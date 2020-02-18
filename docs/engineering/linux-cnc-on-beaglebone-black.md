# Linux CNC on Beaglebone Black

The Beaglebone Black is a good choice for a LinuxCNC SBC because of it's PRUs. The PRUs allow real-time IO control with data shared between the IO processor and user-space memory.

The thing that the Beaglebone is not great at is graphics. Once you add that into the mix you end up with pretty poor performance from the Beaglebone. Instead, it's possible to configure a Remote UI to use a headless Beaglebone running Linux CNC.

## MachineKit

The first stop is getting MachineKit installed on an SD Card to run on a Beaglebone.

Most software these days resides on Github: https://github.com/machinekit
