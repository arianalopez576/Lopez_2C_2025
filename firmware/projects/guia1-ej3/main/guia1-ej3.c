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
#include "freertos/FreeRTOS.h" //para funciones de tiempo real
#include <freertos/task.h> //para las tareas asociadas a freeRTOS
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	struct leds
	{
	uint8_t mode; //ON, OFF, TOGGLE
	uint8_t n_led; //indica el número de led a controlar
	uint8_t n_ciclos; //indica la cantidad de ciclos de encendido/apagado (parpadeo)
	uint16_t periodo; //indica el tiempo de cada ciclo
	} 
}
	//declaro una funcion void para el control de los leds
	//esta funcion tiene como parametro un puntero al struct de leds
	void control_leds (struct leds *my_leds)
	{
		//esta funcion es la que sigue el diagrama de flujos
		//la flecha es un operador de acceso a miembros de un puntero
		//----MODO ON----
		if (my_leds -> mode == ON){
			if (my_leds-> n_led == 1){
				LedOn(1);
			}
			else if (my_leds-> n_led == 2){
				LedOn(2);
			}
			else if (my_leds-> n_led == 3){
				LedOn(3);
			}
		}
	}
/*==================[end of file]============================================*/