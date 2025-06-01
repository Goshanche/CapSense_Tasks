#include "project.h"
#include <stdio.h>
#include <stdlib.h>

#define TIMEOUT_MS 1000
#define IDLE_MS 3000
#define IDLE_TIMEOUT_MS 5000
uint32_t lastActivityTime = 0;
uint8_t inLowPowerMode = 0;
uint8_t resumeFromSleep = 0;
uint8_t resumeTarget = 255;
uint32_t resumeStartTime = 0;
uint32_t resumeTimeoutStart = 0;

volatile uint8_t resetRequested = 0;

uint32_t roundCount = 0;
uint32_t totalReactionTime = 0;

void TurnOffAllLEDs() {
    LED_CapSense_Button_0_Write(1);
    LED_CapSense_Button_1_Write(1);
    LED_CapSense_Button_2_Write(1);
}

void EnterLowPowerMode()
{
    UART_Stop(); // optional: stop UART to save power
    LED_User_status_Write(1); // turn off status LED
    inLowPowerMode = 1;

    UART_UartPutString("Entering Low Power Mode...\r\n");

    while (inLowPowerMode)
    {
        // Start CapSense scan to allow wake-on-touch
        CapSense_ScanAllWidgets();
        while (CapSense_IsBusy());

        CapSense_ProcessAllWidgets();

        if (CapSense_IsAnyWidgetActive())
        {
            inLowPowerMode = 0;
            break;
        }

        if (User_button_pin_Read() == 0)
        {
            inLowPowerMode = 0;
            break;
        }

        CySysPmSleep(); // enter core sleep
    }

    UART_Start(); // restart UART
    UART_UartPutString("Woke from Low Power Mode\r\n");
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

CY_ISR(UserButtonISR)
{
    resetRequested = 1;
    User_button_pin_ClearInterrupt(); // Clear the interrupt flag
}

int main(void)
{
    __enable_irq();

    UART_Init();
    UART_Start();
    CapSense_Start();

    // Initial CapSense scan
    CapSense_ScanAllWidgets();
    while (CapSense_IsBusy());
    CapSense_ProcessAllWidgets();

    // Start User Button interrupt
    User_button_isr_StartEx(UserButtonISR);

    LED_User_status_Write(1);
    UART_UartPutString("Game Ready. Press User Button (SW2) to Start.\r\n");

    for (;;)
    {
        if (resetRequested)
        {
            CyDelay(50); // Debounce
            if (User_button_pin_Read() == 0)
            {
                while (User_button_pin_Read() == 0); // Wait for release
                CyDelay(50);

                lastActivityTime = CySysTickGetValue();
                srand(CySysWdtGetCount());

                UART_UartPutString("Game Started\r\n");
                LED_User_status_Write(1);
                roundCount = 0;
                totalReactionTime = 0;
                resetRequested = 0;

                while (1) // Game loop
                {
                    if (resetRequested) {
                        UART_UartPutString("Game Restarted by Button\r\n");
                        resetRequested = 0;
                        break;
                    }

                    // Idle timeout check
                    if ((CySysTickGetValue() - lastActivityTime) / 1000 > IDLE_TIMEOUT_MS) {
                        EnterLowPowerMode();
                        lastActivityTime = CySysTickGetValue();
                        break;
                    }

                repeat_round:

                    TurnOffAllLEDs();

                    // Wait for all buttons to be released
                    uint32_t releaseTimeout = 2000;
                    uint32_t releaseStart = CySysTickGetValue();
                    uint8_t allReleased = 0;

                    while ((CySysTickGetValue() - releaseStart) < releaseTimeout * 1000) {
                        if (resetRequested) break;

                        CapSense_ScanAllWidgets();
                        while (CapSense_IsBusy());
                        CapSense_ProcessAllWidgets();

                        if (!CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID) &&
                            !CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID) &&
                            !CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) {
                            allReleased = 1;
                            break;
                        }

                        CyDelay(10);
                    }

                    if (!allReleased) {
                        UART_UartPutString("Buttons held too long, skipping round...\r\n");
                        BlinkUserLED();
                        goto repeat_round;
                    }

                    CyDelay(500);
                    lastActivityTime = CySysTickGetValue();

                    // Select random target
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
                            UART_UartPutString("Game Restarted by Button\r\n");
                            resetRequested = 0;
                            hit = 0;
                            goto end_of_round;
                        }

                        CapSense_ScanAllWidgets();
                        while (CapSense_IsBusy());
                        CapSense_ProcessAllWidgets();

                        uint8_t pressed = 255;

                        if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) {
                            pressed = 0;
                        }
                        else if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) {
                            pressed = 1;
                        }
                        else if (CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID)) {
                            pressed = 2;
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
                            lastActivityTime = CySysTickGetValue(); // activity
                            break;
                        }

                        CyDelay(10);
                    }

                end_of_round:
                    TurnOffAllLEDs();

                    if (!hit)
                    {
                        UART_UartPutString("Miss or Timeout! Repeating round...\r\n");
                        BlinkUserLED();
                        OutputStats();
                        goto repeat_round;
                    }

                    CyDelay(500);
                }
            }
        }

        // Fallback idle timeout
        if ((CySysTickGetValue() - lastActivityTime) / 1000 > IDLE_TIMEOUT_MS) {
            EnterLowPowerMode();
            lastActivityTime = CySysTickGetValue();
        }
    }
}