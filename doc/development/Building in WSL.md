## Building in WSL

For various reasons, a custom build of AM32 might be needed. This guide provides the basic instructions needed to build AM32 in Windows Subsystem Linux (WSL).

## Installing the Linux subsystem

Your PC must be running a 64-bit version of Windows 10 build 1607+.

Install the Linux subsystem from [this great guide](https://msdn.microsoft.com/en-us/commandline/wsl/install-win10).

We recommend Ubuntu.

Once you successfully install WSL, enter ``bash`` in Windows command promt, it will show some thing like this:
```
Microsoft Windows [Version 10.0.19042.572]
(c) 2020 Microsoft Corporation. All rights reserved.

C:\Users\fugui>bash
chiang@DESKTOP-6LOVNKK:/mnt/c/Users/fugui$
```

## Configuring the Linux subsystem

We need to install a few Linux packages.

```
sudo apt-get update
sudo apt install git
sudo apt install gcc-arm-none-eabi
```
## Cloning AM32

You can either use git command or GitHub Desktop to clone AM32.
```
cd /mnt/f
mkdir GitHub
git clone https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware.git AM32
```
This will create a ``GitHub`` folder in your ``F:`` drive, and clone (=download) the AM32 repo into it.

## Building AM32

Enter make <targetname> to build AM32 for your chosen board. For example ``make FD6288`` or ``make FD6288 hglrc`` or ``make all``.

The ``.bin`` and ``.elf`` files will be in the main folder of the AM32 folder.

## How to Build Elegantly

1. Install VSCode
2. Install Remote:WSL extention
3. Start the WSL from the VSCode
4. ``ctrl+shift+` `` then ``cd /mnt/f/GitHub/AM32-MultiRotor-ESC-firmware``
5. ``make <target>``
