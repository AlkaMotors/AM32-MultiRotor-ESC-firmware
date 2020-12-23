# AM32-MultiRotor-ESC-firmware
Firmware for STM32F051 based speed controllers for use with mutirotors
<p align="left">
  <a href="/LICENSE"><img src="https://img.shields.io/badge/license-GPL--3.0-brightgreen" alt="GitHub license" /></a>
</p>

The AM32 firmware is designed for STM32 ARM processors to control a brushless motor (BLDC).
The firmware is intended to be safe and fast with smooth fast startups and linear throttle. It is meant for use with multirotor aircraft and a flight controller. The firmware can also be built with support for crawlers.

## Features

AM32 has the following features:

- Firmware upgradable via betaflight passthrough - only support G4, F4 and F7 FCs, and make sure your betaflight firmware is up to date when you trying to use passthrough
- Servo PWM, Dshot(300, 600) motor protocol support
- Bi-directional Dshot
- KISS standard ESC telemetry
- Variable PWM frequency
- Sinusoidal startup mode, which is designed to get larger motors up to speed

## Firmware Release & Configuration Tool

https://github.com/conuthead/AM32-MultiRotor-ESC-firmware/tree/master/Release

To configure AM32 you should use the Multi_ESC config tool (Windows) which using betaflight passthrough capability, which can be found here:

https://github.com/conuthead/AM32-MultiRotor-ESC-firmware/tree/master/Release/CONFIG%20TOOL

## Hardware
Target compatibility List can be found here (DEC 2020)
https://docs.google.com/spreadsheets/d/1NF1Rkq_BFkA53i3A-R1VcAJXwEQCIs_VxcVT_-58-Yg/edit?usp=sharing

## Installation & Documentation

To do...
