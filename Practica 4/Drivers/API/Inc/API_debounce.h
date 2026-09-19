#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"


void debounceFSM_init();
void debounceFSM_update();
bool readKey();

#endif
