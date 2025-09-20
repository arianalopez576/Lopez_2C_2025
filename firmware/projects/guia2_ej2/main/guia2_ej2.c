/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
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

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
TaskHandle_t tarea_led_display = NULL;
TaskHandle_t tarea_teclas = NULL;


bool activacion = true;
bool hold = false;

/*==================[internal functions declaration]=========================*/
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

static void manejo_leds_display (void *puntero_tarea_led){
    while (true){
		if (activacion){
			uint16_t distancia = HcSr04ReadDistanceInCentimeters();
			//distancia representada en los leds
			actualiza_LED(distancia);
			if (!hold){
				//distancia representada en el display
				LcdItsE0803Write(distancia);
			}
		}

	vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
static void manejo_teclas (void *puntero_tarea_teclas){
while (true){
	int8_t teclas;
	teclas = SwitchesRead();
	
	switch(teclas){
	case SWITCH_1:
		activacion = ! activacion;
	break;
	case SWITCH_2:
		hold = ! hold;
	break;		
	}
	vTaskDelay(50/portTICK_PERIOD_MS);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void){
	//inicializaciones
	LedsInit();
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
	SwitchesInit();

	xTaskCreate(&manejo_leds_display, "LED_1", 512, NULL, 5, &tarea_led_display);
	xTaskCreate(&manejo_teclas, "LED_2", 512, NULL, 5, &tarea_teclas);
}
/*==================[end of file]============================================*/