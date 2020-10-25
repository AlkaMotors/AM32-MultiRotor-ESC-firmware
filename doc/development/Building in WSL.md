## Building in Windows

For various reasons, a custom build of AM32 might be needed. This guide provides the basic instructions needed to build AM32 in Windows Subsystem Linux (WSL).

## Installing the Linux subsystem

Your PC must be running a 64-bit version of Windows 10 build 1607+.

You will need to restart your PC at least once, so be mentally prepared for that.

Install the Linux subsystem from [this great guide](https://msdn.microsoft.com/en-us/commandline/wsl/install-win10).

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
sudo apt install git
sudo apt install gcc-arm-none-eabi
```
## Cloning AM32

You can either use git command or GitHub Desktop to clone AM32.




The Windows directory in WSL would be some thing like``mnt/c/GitHib``. Say the Windows directory is ``F:\GitHub\AM32-MultiRotor-ESC-firmware``, the WSL directory should be ``/mnt/f/GitHub/AM32-MultiRotor-ESC-firmware``


2. Install VSCode on Windows, and then install ``Remote - WSL`` extention
3. Click the Open Remote Window button on the left corner, then select ``Remote-WSL: New window``
4. use ``ctrl+shift+` `` to create a new terminal, then ``cd`` to your repo folder 
```cd /<Your directory>/AM32-MultiRotor-ESC-firmware```
for example, my AM32 folder in Windows is ``F:\GitHub\AM32-MultiRotor-ESC-firmware``
 then in WSL the directory will be ``/mnt/f/GitHub/AM32-MultiRotor-ESC-firmware``
5. dfsf

