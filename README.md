# microbit-exposure-simulator
COVID-19 BLE Exposure Notification Simulator for the micro:bit

## What does it do?
The micro:bit sends simulated COVID-19 Exposure Notification beacons. RPI, AEM and bluetooth address change every 10..20 minutes.

LEDs indicate the current tx power setting. Minimum tx power is '1', maximum tx power is '8'. A single dot indicates transmissions have been turned off.

Press *A* to start sending Exposure Notifications or to change the tx power.

Press *A* for 3 seconds to stop sending Exposure Notifications.

Press *B* for 3 seconds to enable random tx power fluctuations. Press again for 3 seconds to switch back to constant tx power.

## How to Build
This project uses yotta to build, not pxt.

```bash
# build the project
yotta build

# transfer the hex file to micro:bit. (for example, macOS X)
cp build/bbc-microbit-classic-gcc/source/microbit-exposure-simulator-combined.hex /Volumes/MICROBIT/
```

## Building on Windows
get https://launchpad.net/gcc-arm-embedded/+download for arm gcc  
get https://github.com/ninja-build/ninja for building with cmake  
get https://sourceforge.net/projects/srecord/ for linking   
