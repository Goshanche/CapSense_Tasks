Validation Use Cases
1. Basic Reaction Time Accuracy
Goal: Confirm that average reaction time is computed correctly.
Test: Play multiple rounds with known delays between LED activation and button press (e.g., press after exactly 300 ms).
Expected Result: Average reaction time should closely match known delays.

2. Correct LED/Button Mapping
Goal: Ensure that the system properly matches the lit LED to the corresponding capacitive button.
Test: Manually verify each LED corresponds to the correct CapSense button.
Expected Result: Pressing the correct button within the timeout should count as a valid response; mismatches or late responses should not.

3. Low Power Wakeup Functionality
Goal: Validate that the device reliably enters and exits low power mode.
Test: Allow the system to idle past the IDLE_TIMEOUT_MS. Then test both CapSense and User Button as wakeup sources.
Expected Result: Device should resume normal operation upon valid input.

4. Debounce and Reset Handling
Goal: Confirm that the User Button ISR properly detects a reset without false triggering.
Test: Press the User Button quickly and repeatedly during gameplay.
Expected Result: Game should reliably reset only when the button is pressed and released intentionally.

5. Held Button Timeout
Goal: Ensure the game does not proceed if buttons are held down.
Test: Power on the system with one or more buttons already pressed.
Expected Result: Game should detect and skip the round or wait for release before continuing.

6. Random Target Distribution
Goal: Validate randomness in LED target selection.
Test: Log the sequence of LED targets over multiple sessions.
Expected Result: Reasonable distribution of targets (0, 1, 2) with no obvious patterns.

7. No Activity Timeout During Gameplay
Goal: Confirm that idle timeout during the main game loop correctly triggers low power mode.
Test: Start the game and leave it inactive during the LED-on phase.
Expected Result: System should enter sleep after defined timeout and wake on valid input.