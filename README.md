# Intelligent Arduino Robot

## Installation
- Download the code from this repo (using `git`, or just download the zip)
- Open the file `line_tracker.ino` from your Arduino IDE. It should load the many files (`.ino`, `.h`, `.cpp`) at the same time.

## Development
The main code is at `line_tracker.ino` (entry point). It imports everything else.

The code to interact with other devices are split into multiple components. Each component has a header file (`.h`) to declare functions and variables, and a definition file (`.cpp`) to define those functions.

We have the following components:
- Motor: control all 4 motors
- Encoder: control the encoder reader (to measure the wheels' rotation)
- IRSensor: the line-tracker with 5 Infra-red sensors
- UltrasonicSensor (or "ultr" sensor): to measure the distance to the object around the robot

## Usage
Just press the Verify or Upload button in Arduino IDE, as usual. 

## Authors
Duong, Khoi, Ngan, Nghi
