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

The latest release of the firmware can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/releases/).

To configure AM32 and upload firmware you can use the Multi_ESC config tool using Betaflight passthrough. They can be downloaded from here:

[WINDOWS](https://drive.google.com/file/d/1o5XP8kSecdH3bPRClUwI7150Od7UdhkG/view?usp=sharing),
[LINUX](https://drive.google.com/file/d/164mF-7khqH822kLzF8sP5VAckHecoKGG/view?usp=sharing)

Alternately you can use the [BlHeli-Configurator](https://github.com/blheli-configurator/blheli-configurator-nightlies/releases/tag/v20201229.20) to flash or change settings. Currently only nightly builds support AM32.



## Hardware
Target compatibility List can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/List-of-Supported-Hardware) (DEC 2020)


## Installation & Documentation

To use AM32 firmware on a blank ESC, a bootloader must first be installed using a ST-LINK. Choose the bootloader that matches the signal input pin of the ESC.
The compatibility chart has the bootloader pinouts listed.
Current bootloaders can be found [here](https://github.com/AlkaMotors/AM32_Bootloader_F051/releases/tag/v5).


## Support and Developers Channel

There's a dedicated Slack chat channel here:

https://am32-invite.herokuapp.com/

Etiquette: Don't ask to ask and please wait around long enough for a reply - sometimes people are out flying, asleep or at work and can't answer immediately.

## Contributors
A big thanks to all those who contributed time, advice and code to the AM32 project.\
Hugo Chiang (Dusking)\
Micheal Keller (Mikeller)\
ColinNiu\
Jacob Walser

And for feedback from pilots and drivers:\
Jye Smith\
Markus Gritsch\
Voodoobrew 

(and many more)
