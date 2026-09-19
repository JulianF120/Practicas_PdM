/**
 * @file API_debounce.h
 * @brief Public interface for the button debounce finite state machine.
 * @author Julian Ferreira
 * @date 19/09/2026
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Initializes the debounce FSM.
 *
 * Sets the initial state to BUTTON_UP and configures the debounce delay.
 * Must be called before debounceFSM_update().
 */
void debounceFSM_init();

/**
 * @brief Updates the debounce FSM.
 *
 * Reads the button GPIO pin and transitions through the FSM states
 * (BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING) to
 * filter out mechanical bounce. Should be called periodically in the main loop.
 */
void debounceFSM_update();

/**
 * @brief Returns whether the button was pressed since the last call.
 *
 * This function implements a read-and-clear mechanism: if the button
 * was pressed, it returns true and clears the internal flag so that
 * subsequent calls return false until a new press is detected.
 *
 * @return true if the button was pressed, false otherwise.
 */
bool readKey();

#endif
