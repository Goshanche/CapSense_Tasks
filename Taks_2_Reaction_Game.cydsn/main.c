#include "project.h"
#include <stdio.h>
#include <stdlib.h>

#define TIMEOUT_MS 1000      // Maximum allowed reaction time
#define IDLE_MS 3000         // Total time to wait for user input before restart

uint32_t roundCount = 0;
uint32_t totalReactionTime = 0;

// Turn off all target leds
void TurnOffAllLEDs() {
    LED_CapSense_Button_0_Write(1);
    LED_CapSense_Button_1_Write(1);
    LED_CapSense_Button_2_Write(1);
}

// Blink user status LED to indicate failure
void BlinkUserLED() {
    LED_User_status_Write(0);
    CyDelay(200);
    LED_User_status_Write(1);
}

// Print game statistics to UART
void OutputStats() {
    char msg[64];
    if (roundCount > 0) {
        sprintf(msg, "Rounds: %lu, Avg Time: %lu ms\r\n",
        (unsigned long)roundCount,
        (unsigned long)(totalReactionTime / roundCount));

        UART_UartPutString(msg);
    }
}

int main(void)
{
    __enable_irq(); 

    // Initialize all hardware components
    UART_Init();
    UART_Start();
    CapSense_Start();
    CapSense_ScanAllWidgets();
    LED_User_status_Write(1); 
    UART_UartPutString("Game Started\n");

    for (;;)
    {
        // Wait for user to press the start button
        if (User_Button_Read() == 0)
        {
            CyDelay(50);
            if (User_Button_Read() != 0) continue;

            // Reset game state
            UART_UartPutString("Game Started\n");
            LED_User_status_Write(1);
            roundCount = 0;
            totalReactionTime = 0;

            while (1) // Game round loop
            {
                // Refresh CapSense data
                CapSense_ProcessAllWidgets();
                CapSense_ScanAllWidgets();
                while (CapSense_IsBusy());

                TurnOffAllLEDs();

                // Select a random target LED and active it
                uint8_t target = rand() % 3;


                switch (target)
                {
                    case 0: LED_CapSense_Button_0_Write(0); break;
                    case 1: LED_CapSense_Button_1_Write(0); break;
                    case 2: LED_CapSense_Button_2_Write(0); break;
                }

                uint32_t startTime = CySysTickGetValue();
                uint32_t timeoutStart = CySysTickGetValue();
                uint8_t hit = 0;

                // Wait for valid user input or timeout
                while ((CySysTickGetValue() - timeoutStart) < IDLE_MS * 1000)
                {
                    CapSense_ProcessAllWidgets();

                    // Check what button is pressed
                    uint8_t pressed = 255;
                    if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) {
                        CyDelay(30);
                        if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) pressed = 0;
                    } else if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) {
                        CyDelay(30);
                        if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) pressed = 1;
                    } else if (CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) {
                        CyDelay(30);
                        if (CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) pressed = 2;
                    }

                    // Handle user response
                    if (pressed != 255)
                    {
                        uint32_t reactionTime = (CySysTickGetValue() - startTime) / 1000;

                        char msg[64];
                        sprintf(msg, "Pressed: %d, Expected: %d, Time: %lu ms\r\n", pressed, target, (unsigned long)reactionTime);
                        UART_UartPutString(msg);

                        // Validate response and update stats
                        if (pressed == target && reactionTime <= TIMEOUT_MS)
                        {
                            roundCount++;
                            totalReactionTime += reactionTime;
                        }
                        else
                        {
                            UART_UartPutString("Incorrect or Late Press!\r\n");
                            BlinkUserLED();
                            OutputStats();
                        }

                        hit = 1;
                        break;
                    }

                    CapSense_ScanAllWidgets();
                    CyDelay(10);
                }

                // Turn off all leds after each round
                TurnOffAllLEDs();

                if (!hit)
                {
                    // No valid input in time
                    UART_UartPutString("Miss or Timeout!\r\n");
                    BlinkUserLED();
                    OutputStats();
                    continue;
                }

                CyDelay(500); // Delay between rounds

                // Check if user wants to restart the game
                if (User_Button_Read() == 0)
                {
                    CyDelay(50);
                    if (User_Button_Read() == 0)
                    {
                        UART_UartPutString("Game Restarted\r\n");
                        break;
                    }
                }
            }
        }
    }
}