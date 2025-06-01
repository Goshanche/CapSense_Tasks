#include "project.h"
#include <stdio.h>
#include <stdlib.h>

#define TIMEOUT_MS 1000
#define IDLE_MS 3000

volatile uint8_t resetRequested = 0;

uint32_t roundCount = 0;
uint32_t totalReactionTime = 0;

void TurnOffAllLEDs() {
    LED_CapSense_Button_0_Write(1);
    LED_CapSense_Button_1_Write(1);
    LED_CapSense_Button_2_Write(1);
}

void BlinkUserLED() {
    LED_User_status_Write(0);
    CyDelay(200);
    LED_User_status_Write(1);
}

void OutputStats() {
    char msg[64];
    if (roundCount > 0) {
        sprintf(msg, "Rounds: %lu, Avg Time: %lu ms\r\n",
            (unsigned long)roundCount,
            (unsigned long)(totalReactionTime / roundCount));
        UART_UartPutString(msg);
    }
}

/*
// Optional ISR for reset button
CY_ISR(UserResetISR) {
    if (User_button_pin_Read() == 1) {
        CyDelay(50);
        if (User_button_pin_Read() == 1) {
            while (User_button_pin_Read() == 1);
            CyDelay(50);
            if (roundCount > 0)
                resetRequested = 1;
        }
    }
}
*/

int main(void)
{
    __enable_irq();

    UART_Init();
    UART_Start();
    CapSense_Start();
    CapSense_ScanAllWidgets();
    
    srand(CySysTickGetValue());

    LED_User_status_Write(1);
    UART_UartPutString("Game Ready. Press User Button (SW2) to Start.\r\n");

    // isr_UserReset_StartEx(UserResetISR); // Optional: Enable ISR

    for (;;)
    {
        // Wait for user to press the start button with debounce
        if (User_button_pin_Read() == 0)
        {
            CyDelay(50);
            if (User_button_pin_Read() == 0)
            {
                while (User_button_pin_Read() == 0); // Wait for release
                CyDelay(50);

                // Reset game state
                UART_UartPutString("Game Started\n");
                LED_User_status_Write(1);
                roundCount = 0;
                totalReactionTime = 0;
                resetRequested = 0;

                while (1) // Game round loop
                {
                    if (resetRequested) {
                        UART_UartPutString("Game Restarted by Reset Button\r\n");
                        resetRequested = 0;
                        break;
                    }

                    CapSense_ProcessAllWidgets();
                    CapSense_ScanAllWidgets();
                    while (CapSense_IsBusy());

                    TurnOffAllLEDs();

                    // Wait for all buttons to be released
                    uint32_t releaseTimeout = 2000;
                    uint32_t releaseStart = CySysTickGetValue();
                    uint8_t allReleased = 0;

                    while ((CySysTickGetValue() - releaseStart) < releaseTimeout * 1000) {
                        CapSense_ProcessAllWidgets();
                        if (!CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID) &&
                            !CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID) &&
                            !CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) {
                            allReleased = 1;
                            break;
                        }
                        CapSense_ScanAllWidgets();
                        CyDelay(10);
                    }

                    if (!allReleased) {
                        UART_UartPutString("Buttons held too long, skipping round...\r\n");
                        BlinkUserLED();
                        continue;
                    }

                    CyDelay(500);

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

                    while ((CySysTickGetValue() - timeoutStart) < IDLE_MS * 1000)
                    {
                        if (resetRequested) {
                            UART_UartPutString("Game Restarted by Reset Button\r\n");
                            resetRequested = 0;
                            hit = 0;
                            goto end_of_round;
                        }

                        CapSense_ProcessAllWidgets();

                        uint8_t pressed = 255;

                        if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) {
                            CyDelay(20);
                            if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) pressed = 0;
                        } 
                        else if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) {
                            CyDelay(20);
                            if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) pressed = 1;
                        } 
                        else if (CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) {
                            CyDelay(20);
                            if (CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) pressed = 2;
                        }

                        if (pressed != 255)
                        {
                            uint32_t reactionTime = (CySysTickGetValue() - startTime) / 1000;

                            char msg[64];
                            sprintf(msg, "Pressed: %d, Expected: %d, Time: %lu ms\r\n",
                                pressed, target, (unsigned long)reactionTime);
                            UART_UartPutString(msg);

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

                end_of_round:
                    TurnOffAllLEDs();

                    if (!hit)
                    {
                        UART_UartPutString("Miss or Timeout!\r\n");
                        BlinkUserLED();
                        OutputStats();
                        continue;
                    }

                    CyDelay(500);
                }
            }
        }
    }
}
