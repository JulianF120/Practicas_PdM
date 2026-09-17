/*
 * API_delay.c
 *
 *  Created on: 10 set. 2026
 *      Author: julia
 */

#include "API_delay.h"

/*
 * parametros de entrda:
 * delay: puntero del tipo delay_t, se seta running en false
 * duration: duracion en ms para setear en delay
 */
void delayInit( delay_t * delay, tick_t duration ){

	if (delay == NULL || duration == 0) {
		return;
	}

	delay->running = false;
	delay->duration = duration;
}

/*
 * parametros de entrda:
 * delay: puntero del tipo delay_t
 * la funcion devuelve un bool dependiendo de si el tiempo de duracion establecido en
 * la variable se cumplio o no
 */
bool_t delayRead( delay_t * delay ){

	if (delay == NULL) {
	        return false;
	}

	tick_t time = HAL_GetTick();

	if (!delay->running){
		delay->running = true;
		delay->startTime = time;
	}
	else {
		if (HAL_GetTick() - delay->startTime > delay->duration){
			delay->running = false;
			return true;
		}
	}
	return false;
}

/*
 * parametros de entrda:
 * delay: puntero del tipo delay_t
 * duration: tiempo en ms a setear en delay
 */
void delayWrite( delay_t * delay, tick_t duration ){

	if (delay == NULL || duration == 0) {
	        return;
	}

	delay->duration = duration;
}

bool_t delayIsRunning(delay_t * delay){
	return delay->running;
}
