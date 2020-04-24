# Capstone 

## Background
This repository provides the firmware necessary for a Smart AWD system for McMaster Baja Racing's vehicle. 

This is a project to be used with the MCUXPresso IDE. 

The code is generated for the FRDM-K64F development board. If on McMaster Baja Racing, see the associated documentation on Box for background, wiring schematics, verification, and validation. 

## Software Design
- All custom, non-firmware code is in [source](/source/). 
- The main loop is found in MK64FN1M0xxx12_Project.cpp. This handles all initializations and at what rate every function is called at. 
- All IRQ handling is done in IrqHandler.h/.c. 
- Simulink code generation is used to create AWDControl.c/.h and its associated header files. 
  - The Simulink code was designed to be called at a 1kHz rate. 
- Each sensor has an associated initialization function and sensor getter function in its own .c or .cpp file:
  - accelerometer.h/.cpp for 6-Axis IMU
  - gps.c/.h for velocity values from the GPS
  - potentiometer.c/.h for steering wheel angle values
  - manual_controls.c/.h for all manual input from steering wheel buttons
  - axle_rpm.c/.h for axle rpm values from hall effect sensors



