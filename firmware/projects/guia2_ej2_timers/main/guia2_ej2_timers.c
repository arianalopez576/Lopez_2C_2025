/*! @mainpage Proyecto 2 Ejercicio 2 utilizando timers
 *
 * @section genDesc General Description
 *
 * El programa tiene una tarea notificada por un temporizador, además a través de interrupciones 
 * se realiza el manejo de teclas. 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 			 	| 	GPIO_9		|
 * |				|	GPIO_18		|
 * |				|	GPIO_19		|
 * |	 LCD		|	GPIO_20		|
 * |				|	GPIO_21		|
 * |				|	GPIO_22		|
 * |				|	GPIO_23		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 24/10/2023 | Se documenta 			                         |
 * | 24/10/2025 | Se finaliza la documentacion 					 |
 *
 * @author Ariana Lopez (lopezariana576@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "switch.h"
#include "timer_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
bool activacion = true;
bool hold = false;

TaskHandle_t tarea_led_display = NULL;
TaskHandle_t tarea_tecla_1 = NULL;
TaskHandle_t tarea_tecla_2 = NULL;

/*==================[internal functions declaration]=========================*/
/** 
 * @brief Enciende o apaga los distintos LEDs dependiendo del valor de distancia medido
 * @param distancia_cm Dato medido por el sensor
*/
void actualiza_LED (uint16_t distancia_cm){
	if (distancia_cm < 10){
		LedOff(LED_1);
		LedOff(LED_2);
		LedOff(LED_3);
	}
	else if (distancia_cm >= 10 && distancia_cm < 20){
		LedOn(LED_1);
	}
	else if (distancia_cm >=20 && distancia_cm < 30){
		LedOn(LED_1);
		LedOn(LED_2);
	}
	else if (distancia_cm >= 30){
		LedOn(LED_1);
		LedOn(LED_2);
		LedOn(LED_3);
	}
}

/** 
 * @brief Modifica el encendido/apagado de LEDs y el manejo de las teclas
 * @param puntero_tarea_led Puntero a un arreglo que contiene la tarea de leds y dissplay
*/
static void manejo_leds_display (void *puntero_tarea_led){
    while (true){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (activacion){
			uint16_t distancia = HcSr04ReadDistanceInCentimeters();
			//distancia representada en los leds
			actualiza_LED(distancia);
			if (!hold){
				//distancia representada en el display
				LcdItsE0803Write(distancia);
			}
		}
	}
}

//Funciones para enviar una notificacion
/** 
 * @brief Envia una notificacion
 * @param param Puntero a un arreglo
*/
void FuncTimerA(void *param){
    vTaskNotifyGiveFromISR(tarea_led_display, pdFALSE);
}
/** 
 * @brief Modifica la variable que activa/desactiva la medicion del sensor
 * @param puntero_tarea_tecla_1 Puntero a la tarea de la tecla 1
*/
static void manejo_tecla_1 (void *puntero_tarea_tecla_1){ 
		activacion = ! activacion;
}
/** 
 * @brief Modifica la variable que congela el valor mostrado en el display
 * @param puntero_tarea_tecla_1 Puntero a la tarea de la tecla 1
*/
static void manejo_tecla_2 (void *puntero_tarea_tecla_2){
		hold = ! hold;	
}

/*==================[external functions definition]==========================*/
void app_main(void){
	LedsInit();
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
	//interrupciones teclas
	SwitchesInit();
	SwitchActivInt(SWITCH_1, manejo_tecla_1, NULL);
	SwitchActivInt(SWITCH_2, manejo_tecla_2, NULL);

	//Inicializacion de timer
	timer_config_t timer_tarea_led_display = {
        TIMER_A,
        50000,
        FuncTimerA,
        NULL
    };
	TimerInit(&timer_tarea_led_display);
	
	//tarea
	xTaskCreate(&manejo_leds_display, "LED_1", 512, NULL, 5, &tarea_led_display);

    //timers
    TimerStart(timer_tarea_led_display.timer);
}	
/*==================[end of file]============================================*/