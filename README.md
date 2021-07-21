# AM32-MultiRotor-ESC-firmware
Firmware for STM32F051 based speed controllers for use with multirotors
<p align="left">
  <a href="/LICENSE"><img src="https://img.shields.io/badge/license-GPL--3.0-brightgreen" alt="GitHub license" /></a>
</p>

The AM32 firmware is designed for STM32 ARM processors to control a brushless motor (BLDC).
The firmware is intended to be safe and fast with smooth fast startups and linear throttle. It is meant for use with multirotor aircraft and a flight controller. The firmware can also be built with support for crawlers.

A note for manufacturers : There have been an increasing number of request for support for commercial ESC's . New targets can be added to the targets.h file. Sending ESC's will NOT get your target supported. I have no intention of trying to reverse engineer a schematic. Full schematics and a cash donation are required to get proper commerical support. Without this do not even ask.. You are more than welcome to add the target to the firmware on your own.  

## Features

AM32 has the following features:

- Firmware upgradable via betaflight passthrough - only support G4, F4 and F7 FCs, and make sure your betaflight firmware is up to date when you try to use passthrough
- Servo PWM, Dshot(300, 600) motor protocol support
- Bi-directional Dshot
- KISS standard ESC telemetry
- Variable PWM frequency
- Sinusoidal startup mode, which is designed to get larger motors up to speed

## Firmware Release & Configuration Tool

The latest release of the firmware can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/releases/).

To configure AM32 and upload firmware you can use the Multi_ESC config tool using Betaflight passthrough. They can be downloaded from here:

[WINDOWS](https://drive.google.com/file/d/1H2Ks_TOZGshxZ2eOfaa94-t4zi-acqGg/view?usp=sharing),
[LINUX](https://drive.google.com/file/d/164mF-7khqH822kLzF8sP5VAckHecoKGG/view?usp=sharing)

Alternately you can use the [Online-ESC Configurator](https://esc-configurator.com/) to flash or change settings with any web browser that supports web serial.



## Hardware
Target compatibility List can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/List-of-Supported-Hardware) (APR 2021)


## Installation & Bootloader

To use AM32 firmware on a blank ESC, a bootloader must first be installed using an ST-LINK. Choose the bootloader that matches the signal input pin of the ESC.
The compatibility chart has the bootloader pinouts listed.
Current bootloaders can be found [here](https://github.com/AlkaMotors/AM32_Bootloader_F051/releases).

After the bootloader has been installed the main firmware from can be installed either with the configuration tools and a Betaflight flight controller or a direct connection with a usb serial adapter modified for one wire. A walkthrough can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/Crawler-AM32--single-ESC-Flashing-Tutorial).

To update an existing AM32 bootloader an update tool can be found [here](https://github.com/AlkaMotors/F051_Bootloader_Updater/releases).

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
