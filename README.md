# CapSense_Tasks

This repository contains two separate embedded C projects developed for PSoC 4000S microcontroller using CapSense technology. Each project demonstrates a different interactive use of capacitive touch inputs and UART communication.

Project Task_1: 
This project reads the position of a linear CapSense slider and lights up one of five LEDs based on the detected touch position. The output is also printed to the UART console.
Ensure UART is configured at the default baud rate (115200).

Project Task_2_Reaction_Game
This project implements a simple reaction-based game using capacitive touch buttons and LEDs. A random LED is activated, and the player must touch the corresponding capacitive button as quickly and accurately as possible.

How to Play
- Open a UART terminal (e.g., PuTTY or TeraTerm) after flashing the device.
- Press the user button to start the game(SW2 on PsoC 4000S).
- A random LED will turn on — tap the corresponding CapSense button.
- If the correct button is pressed in time, the reaction is counted.
- If not — the error LED blinks, and stats are printed via UART.
- Press the user button again to restart the game.
