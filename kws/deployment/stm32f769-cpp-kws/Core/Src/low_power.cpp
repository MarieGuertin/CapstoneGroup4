/*
 * low_power.c
 *
 *  Created on: Dec. 19, 2021
 *      Author: chrismartel
 */
#include "main.h"
#include "low_power.h"

void enter_sleep_mode(){

	// disable the systick interrupt or else this interrupt will wake the mcu every time it is triggered
	HAL_SuspendTick();
	NVIC_DisableIRQ(TIM2_IRQn);
//	NVIC_DisableIRQ(TIM1_TRG_COM_TIM13_IRQn);
	// enter sleep mode with wait for interrupt (wfi)
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	NVIC_EnableIRQ(TIM2_IRQn);
//	NVIC_EnableIRQ(TIM1_TRG_COM_TIM13_IRQn);
	HAL_ResumeTick();
}
