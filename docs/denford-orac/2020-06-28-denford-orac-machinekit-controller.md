# Denford Orac Machinekit Controller

Finally getting around to setting up a new controller for the Denford Orac. The only original
electronics left are the stepper motors and the spindle index opto detectors for spindle
indexing.

## The Controller

I built a controller in a 3U 19" rack enclosure. I left the VFD for spindle speed control
outside of the enclosure because it's sizeable and I'd like to more easily swap out the VFD
at a later date if necessary.

## Configuring Machinekit

It's like you need a PhD in order to understand how to configure the HAL and INI files of
Machinekit (which is just a Beaglebone Black fork of LinuxCNC). We really need an
easy-to-configure CNC controller that isn't this complete nightmare to configure.

However, until then, Machinekit it is and I'm running it from the eMMC instead of from an
SD Card.

For the Denford Orac axis, I have the following:

- Z axis - 5mm pitch lead screw with a 1:1.75 pulley ratio
- X axis - 2.5mm pitch lead screw with a 1:1.25 pulley ratio

The stepper drivers I've configured to have 800 steps per revolution (for a 1.8deg motor). The denford orac stepper motors are 1.8 deg.

All of this is required for setting up the Machinekit scale INI file settings.

Thje
The scale is set as:

- Z axis - ( 800 / 5 ) * 1.75 = 280
- X axis - ( 800 / 2.5 ) * 1.25 = 400

## Spindle

The spindle is driven by a VFD which has a 0-10V analogue input. The spindle also
provides index and position (not absolute) pulses to the controller so the lathe
can do canned cycles like thread cutting in closed loop.
