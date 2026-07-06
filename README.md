# Code_final

This project is an Arduino/embedded application for a Whack-a-Mole style game with pneumatic actuators, 3D magnetic sensors, and a 2-digit segment display.

## What it does

The sketch controls a game in which random pistons/actuators move up one after another. A hit is detected through the magnetic sensors. At the same time, a countdown runs and is shown on the display. At the end of a round, the hit counter and game state are reset.

## Project structure

The logic is split across several files:

- `Code_final.ino` is the entry point. `setup()` and `loop()` run here. The display, sensors, compressor, and game start are coordinated from this file.
- `display.hpp` initializes the 7-segment display and contains functions for showing numbers and clearing the display.
- `3d_sensor.hpp` initializes the four TLx493D 3D magnetic sensors, reads measurements, and calculates averages and hit comparisons.
- `push.hpp` sets up the TLE94112 motor driver and starts the individual actuators and the compressor.
- `game_matirx.hpp` manages which pistons are currently active and selects the next random piston.
- `airpressure_sensor.hpp` contains an alternative/additional approach using an air-pressure sensor for height or hammer detection. This part is currently commented out in the main sketch.
- `test_blinky/test_blinky.ino` is a separate test program.

## Required hardware

- XMC4700_Relax_Kit(Lite)
- 4x TLx493D 3D magnetic sensors
- TLE94112-based motor driver for the nine actuators and the compressor
- 7-segment display with shift-register control
- Button on pin 77 as the start signal
- Buzzer on pin 76
- Compressor or compressed-air supply for the actuators

## Libraries

- `TLx493D_inc.hpp`
- `tle94112-ino.hpp`
- `tle94112-motor-ino.hpp`
- `Dps3xx.h` for the air-pressure sensor part

## Usage

1. Open the `Code_final` folder in the Arduino IDE or another compatible development environment.
2. Make sure all required libraries are installed.
3. Select the XMC4700_Relax_Kit board and the correct port.
4. Upload `Code_final.ino` to the board.
5. After startup, the sketch initializes the display, sensors, and motor driver. The game then waits for the side button to be pressed.
6. Once the game starts, a 5-second countdown runs and then the game begins. Each hit scores one point. A point is counted when the magnetic sensor detects a change in the magnetic field.

## Important parameters

The sensitivity is controlled by `if(compare_difference(piston_now, 0.15)==true)`. `0.15` is an absolute magnetic-field threshold. If the change is large enough, it is recognized as a hit. The larger the value, the less likely a false hit becomes. However, if the value is too high, real hits may no longer be detected. Choose the value carefully.

The game duration is set through the `gametime` variable, which is currently configured to 15 seconds.


