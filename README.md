# AM32-MultiRotor-ESC-firmware
Firmware for stm32f051 based speed controllers for use with mutirotors

The AM32 firmware is designed for stm32 arm processors to control a brushless motor (bldc).
The firmware is intended to be safe and fast with smooth fast startups and linear throttle. It is meant for use with multirotor aircraft and a flight controller. The firmware can also be built with support for crawlers.

Currently the supported protocols are dshot 300 and dshot 600 and Servo PWM signal.  The Supported loop times are 3.3k, 4k , 6.6k and 8k. 

Bi directional dshot for RPM filtering is supported .

Telemetry output is available on the serial line for esc's that have telemetry output.  Not all sensor information is implemented with telemetry, some ESC do not have current/voltage sense.  

Servo pwm also is enabled with an input range of 1000-2000us and a center of 1500us for dual-direction mode. 

Dshot commands for reversing motor direction, enabling 3d mode and saving , beacons 1-3 are working. 

PWM rate can be varied from 24khz -48khz. Alternately an option can be chosen to let the esc vary the pwm rate proportionally to rpm once the motor has reached a rpm threshold. This keeps the PWM frequency just a little higher than twice commutation frequency for the most used throttle range.

Complemetary pwm (braking) can be turned on or off and is ON by default.

Startup power and minimum pwm can be varied with the slider in the config tool.

By default a low rpm power protection feature is on, the KV rating of the motor is used to decide the level of protection at a given rpm. 

A sinusoidal startup mode is also avaliable to get larger motors up to speed. Currently the speed range is fixed and is proportional to the throttle for the first ten percent of the throttle range.  Only for experimental use curently, meant for lower kv motors (<1200kv).

Motor timing advance can be adjusted in 7.5 degree increments. The default position is 15 degrees but some motors might need higher or lower timing .

All options can be changed with the Multi_ESC config tool using betaflight passthrough capability. Currently compatibility with passthough is supported on G4,  F4 and F7 boards only. Ensure that betafligth is up to date before connecting and flashing.  New firmware revisions can also be installed using the ESC config tool and passthrough. 
