/**
 * @file API_debounce.c
 * @brief Implementation of the button debounce finite state machine.
 * @details Uses a four-state FSM (BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN,
 *          BUTTON_RAISING) combined with a non-blocking delay to filter
 *          mechanical bounce on the user button (B1).
 * @author Julian Ferreira
 * @date 19/09/2026
 */

#include "API_debounce.h"
#include "API_delay.h"

#define B1_Pin GPIO_PIN_13      /**< @brief User button pin. */
#define B1_GPIO_Port GPIOC      /**< @brief User button GPIO port. */

#define LD2_Pin GPIO_PIN_5      /**< @brief On-board LED pin. */
#define LD2_GPIO_Port GPIOA     /**< @brief On-board LED GPIO port. */

/**
 * @brief Possible states of the debounce FSM.
 */
typedef enum{
	BUTTON_UP,       /**< Button is released (idle). */
	BUTTON_FALLING,  /**< Falling edge detected, waiting for debounce confirmation. */
	BUTTON_DOWN,     /**< Button is confirmed pressed. */
	BUTTON_RAISING,  /**< Rising edge detected, waiting for debounce confirmation. */
} debounceState_t;

/**
 * @brief Callback executed when a valid button press is confirmed.
 */
static void buttonPressed();

/**
 * @brief Callback executed when a valid button release is confirmed.
 */
static void buttonReleased();

static debounceState_t currentState;  /**< @brief Current state of the FSM. */
static delay_t bounceDealy;           /**< @brief Non-blocking delay used for debounce timing. */

static uint8_t debounceTime = 40;     /**< @brief Debounce time in milliseconds. */

bool_t keyWasPress = false;           /**< @brief Flag indicating a confirmed key press event. */

/**
 * @brief Initializes the debounce FSM.
 *
 * Configures the debounce delay and sets the initial state to BUTTON_UP.
 */
void debounceFSM_init()
{
	delayInit(&bounceDealy, debounceTime);
	currentState = BUTTON_UP;
}

/**
 * @brief Updates the debounce FSM based on the current button state.
 *
 * Reads the B1 button GPIO pin and transitions through the FSM states.
 * When a stable press or release is confirmed after the debounce delay,
 * the corresponding callback (buttonPressed / buttonReleased) is invoked.
 */
void debounceFSM_update()
{
	switch (currentState)
	{
		case BUTTON_UP:
			if (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			{
				currentState = BUTTON_FALLING;
			}
		break;

		case BUTTON_FALLING:
			if (delayRead(&bounceDealy))
			{
				if (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
				{
					buttonPressed();
					currentState = BUTTON_DOWN;
				}
				else
				{
					currentState = BUTTON_UP;
				}
			}
		break;

		case BUTTON_DOWN:
			if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			{
				currentState = BUTTON_RAISING;
			}
		break;

		case BUTTON_RAISING:
			if (delayRead(&bounceDealy))
			{
				if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
				{
					currentState = BUTTON_UP;
					buttonReleased();
				}
				else
				{
					currentState = BUTTON_DOWN;
				}
			}
		break;
	}
}

/**
 * @brief Sets the keyWasPress flag to indicate a confirmed press.
 */
static void buttonPressed()
{
	keyWasPress = true;
}

/**
 * @brief Clears the keyWasPress flag to indicate a confirmed release.
 */
static void buttonReleased()
{
	keyWasPress = false;
}

/**
 * @brief Checks if the button was pressed and clears the flag.
 *
 * Implements a read-and-clear mechanism: returns true once per press event,
 * then returns false until the next confirmed press.
 *
 * @return true if a press event occurred since the last call, false otherwise.
 */
bool_t readKey()
{
	if (keyWasPress)
	{
		keyWasPress = false;
		return true;
	}
	return keyWasPress;
}
