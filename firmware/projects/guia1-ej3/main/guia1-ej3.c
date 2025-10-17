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
#include <stdbool.h> //para utilizar true/false
#include "led.h"
#include "switch.h"
#include "freertos/FreeRTOS.h" //para funciones de tiempo real
#include <freertos/task.h> //para las tareas asociadas a freeRTOS
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
struct leds
	{
	uint8_t mode; //ON, OFF, TOGGLE
	uint8_t n_led; //indica el número de led a controlar
	uint8_t n_ciclos; //indica la cantidad de ciclos de encendido/apagado (parpadeo)
	uint16_t periodo; //indica el tiempo de cada ciclo
	};

#define ON 1
#define OFF 2
#define TOGGLE 3

/*==================[internal functions declaration]=========================*/
//declaro una funcion void para el control de los leds
//esta funcion tiene como parametro un puntero al struct de leds
	void control_leds (struct leds *my_leds){
		//la flecha es un operador de acceso a miembros de un puntero
		//----MODO ON----
		if (my_leds -> mode == ON){
			if (my_leds -> n_led == LED_1){
				LedOn(LED_1);
			}
			else if (my_leds -> n_led == LED_2){
				LedOn(LED_2);
			}
			else if (my_leds -> n_led == LED_3){
				LedOn(LED_3);
			}
		}
		//----MODO OFF---
		else if (my_leds -> mode == OFF){
			if (my_leds -> n_led == LED_1){
				LedOff(LED_1);
			}
			else if (my_leds -> n_led == LED_2){
				LedOff(LED_2);
			}
			else if (my_leds -> n_led == LED_3){
				LedOff(LED_3);
			}
		}
		//----MODO TOGGLE---
		else if (my_leds -> mode == TOGGLE){
			for (int i = 0; i < my_leds -> n_ciclos; i++){
				if (my_leds -> n_led == LED_1){
					LedToggle(LED_1);
				}
				else if (my_leds -> n_led == LED_2){
					LedToggle(LED_2);
				}
				else if (my_leds -> n_led == LED_3){
					LedToggle(LED_3);
				}
			vTaskDelay(my_leds -> periodo / portTICK_PERIOD_MS);
			}
		}
	}

/*==================[external functions definition]==========================*/

void app_main(void){
	LedsInit();
	struct leds mi_led;

	mi_led.mode = TOGGLE;
	mi_led.n_led = LED_1;
	mi_led.n_ciclos = 10;
	mi_led.periodo = 500; //estos son milisegundos
	control_leds(&mi_led); //& inidica la direccion de memoria del struct
}


/*==================[end of file]============================================*/