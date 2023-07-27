# AM32-MultiRotor-ESC-firmware
Firmware for ARM based speed controllers
<p align="left">
  <a href="/LICENSE"><img src="https://img.shields.io/badge/license-GPL--3.0-brightgreen" alt="GitHub license" /></a>
</p>

The AM32 firmware is designed for STM32 ARM processors to control a brushless motor (BLDC).
The firmware is intended to be safe and fast with smooth fast startups and linear throttle. It is meant for use with multiple vehicle types and a flight controller. The firmware can also be built with support for crawlers. For crawler usage please read this wiki page [Crawler Hardware](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/Crawler-Hardware-and-AM32)

## Features

AM32 has the following features:

- Firmware upgradable via betaflight passthrough, single wire serial or arduino
- Servo PWM, Dshot(300, 600) motor protocol support
- Bi-directional Dshot
- KISS standard ESC telemetry
- Variable PWM frequency
- Sinusoidal startup mode, which is designed to get larger motors up to speed
## Build instructions
-"make" - builds all targets
-"make AM32_XXX_F051 builds" a speciic target
-Github actions
-Fork project and run the build action within github itself.

## Firmware Release & Configuration Tool

The latest release of the firmware can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/releases/).

To configure AM32 and upload firmware you can use the Multi_ESC config tool using Betaflight passthrough. They can be downloaded from here:

[WINDOWS](https://drive.google.com/file/d/1qw2f7nM6OZftIE5GTgMPWB5zXCELxYG9/view?usp=sharing),
[LINUX](https://drive.google.com/file/d/1QtSKwp3RT6sncPADsPkmdasGqNIk68HH/view?usp=sharing)

Alternately you can use the [Online-ESC Configurator](https://esc-configurator.com/) to flash or change settings with any web browser that supports web serial.



## Hardware
AM32 currently has support for STSPIN32F0, STM32F051, STM32G071, GD32E230, AT32F415 and AT32F421.
The CKS32F051 is not recommended due to too many random issues.
Target compatibility List can be found [here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/List-of-Supported-Hardware) (APR 2021)


## Installation & Bootloader

To use AM32 firmware on a blank ESC, a bootloader must first be installed using an ST-LINK, GD-LINK , CMIS-DAP or AT-LINK.  THe bootloader will be dependant on the MCU used ont he esc . Choose the bootloader that matches the MCU type and signal input pin of the ESC.
The compatibility chart has the bootloader pinouts listed.
Current bootloaders can be found [here](https://github.com/AlkaMotors/AM32_Bootloader_F051/tree/main/Bootloaders).

After the bootloader has been installed the main firmware from can be installed either with the configuration tools and a Betaflight flight controller or a direct connection with a usb serial adapter modified for one wire.

To update an existing AM32 bootloader an update tool can be found [here](https://github.com/AlkaMotors/F051_Bootloader_Updater/releases).

## Support and Developers Channel
There are two ways you can get support or participate in improving am32.
We have a discord server here:

https://discord.gg/h7ddYMmEVV

There's a dedicated Slack chat channel here:

https://am32-invite.herokuapp.com/

Etiquette: Please wait around long enough for a reply - sometimes people are out flying, asleep or at work and can't answer immediately. 

If you wish to support the project please join the Patreon.

https://www.patreon.com/user?u=44228479


## Sponsors
The AM32 project would not have made this far without help from the following sponsors:

Holmes Hobbies - https://holmeshobbies.com/ - The project would not be where it is today without the support of HH. Check out the Crawlmaster V2 for the best am32 experience!

Quaternium - https://www.quaternium.com/ - Firmware development support and hardware donations

Airbot - Many hardware donations

NeutronRC - For hardware, am32 promotion and schematics 

Aikon - Hardware donations and schematics\
Skystars  - For hardware and taking a chance on the first commercial am32 esc's\
Diatone - Hardware donations\
T-motor - Motor and Hardware donations\
HLGRC  - Hardaware donations

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
