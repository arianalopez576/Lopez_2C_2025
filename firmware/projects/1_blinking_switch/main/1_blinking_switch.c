/*! @mainpage Blinking switch
 *
 * \section genDesc General Description
 *
 * This example makes LED_1 and LED_2 blink if SWITCH_1 or SWITCH_2 are pressed.
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
#include <stdio.h> //se usa para entrada/salida, no se usa en este caso
#include <stdint.h> //permite definir el tipo uint8_t
#include <stdbool.h> //para variables del tipo true/false
#include "freertos/FreeRTOS.h" //para la constante portickperiod
#include "freertos/task.h" //para el vtaskdelay
#include "led.h" //funciones led 
#include "switch.h" //funciones teclas

/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	uint8_t teclas;
	LedsInit();
	SwitchesInit();
    while(1)    {
    	teclas  = SwitchesRead();
    	switch(teclas){
    		case SWITCH_1:
    			LedToggle(LED_1);
    		break;
    		case SWITCH_2:
    			LedToggle(LED_2);
    		break;
    	}
	    LedToggle(LED_3);
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}
