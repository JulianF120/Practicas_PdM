/**
 * @file API_debounce.c
 * @brief file to manage debounce of buttons.
 * @details ***
 * @author Julian Ferreira
 * @date 19/09/2026
 */



#include "API_debounce.h"
#include "API_delay.h"

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static void buttonPressed();
static void buttonReleased();

static debounceState_t currentState;
static delay_t bounceDealy;

static uint8_t debounceTime = 40; //The unit is miliseconds (ms)

bool_t keyWasPress = false;

void debounceFSM_init()
{
	delayInit(&bounceDealy, debounceTime);
	currentState = BUTTON_UP;
}

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

static void buttonPressed()
{
	keyWasPress = true;
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
}

static void buttonReleased()
{
	keyWasPress = false;
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
}

bool_t readKey()
{
	if (keyWasPress)
	{
		keyWasPress = false;
		return true;
	}
	return keyWasPress;
}

