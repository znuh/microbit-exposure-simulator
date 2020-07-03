# microbit-exposure-simulator
COVID-19 BLE Exposure Notification Simulator for the micro:bit

## How to Build
This project uses yotta to build, not pxt.

# build the project
yotta build

# transfer the hex file to micro:bit. (for example, macOS X)
cp build/bbc-microbit-classic-gcc/source/microbit-exposure-simulator-combined.hex /Volumes/MICROBIT/
```

## Building on Windows
get https://launchpad.net/gcc-arm-embedded/+download for arm gcc  
get https://github.com/ninja-build/ninja for building with cmake  
get https://sourceforge.net/projects/srecord/ for linking   
