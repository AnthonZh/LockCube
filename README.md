# LockCube
This project contains the codebase for LockCube, a submission to HackDuke 2026. This repository organizes the various components of LockCube, a GameCube-inspired poject which reminds us to do the thing we all hate: putting down our phones.

## Inspiration
LockCube was inspired by the increasingly ubiquitous presence of scrolling short-form video content in the lives of teenagers across the US. The dominance of scrolling and its negative impact on the mental health of teenagers encouraged us to look inwards to find things that bring us joy. 

## How it works
The Cube is placed with the camera pointing towards the user. Using multiple YOLO computer vision models, the Cube detect whether the user is doomscrolling on their phone or not. If it detects they are scrolling, the Cube vocally reminds the user, through Gemini API and ElevenLabs API, to be more productive. Then the Cube prompts the user to play a short platform game, increasing their mental health.

## Features
The Cube features a Raspberry Pi, augumented by a camera and Bluetooth speaker. The Raspberry Pi hosts two servers, one that runs comoputer vision and the other that handles voice. 

## Setup
This GitHub repository is designed to organize the different components which goes into building LockCube. The materials required are:
- Raspberry Pi 5
- Potentiometer
- Joystick
- Camera
- STM32F411 BlackPill
- Breadboard
- Assorted cabling and wires
- Galaxy S25 Ultra (for display)

Using the [assembly files](./LockCubeAssembly/), 3D print the camera & gamecube. The STM32F411 BlackPill should have the program from [stm32-joystick](./stm32-joystick/) uploaded. The program in the submodule [LockCubeMiniGame](./LockCubeMiniGame/) has detailed instructions on how to install, and then this should be built to Linux using Unity. The resulting build should be transferred to the Raspberry Pi. The Pi requires `box64` installed to run the Unity build, as well as the [backend](./backend/) running.

## AI Acknowledgement
Large language models like Claude, ChatGPT, Google Gemini, and OpenAI Codex were used to assist in writing some of the code. The [game](./LockCubeMiniGame/) contains additional information on how AI was used in that repository.