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
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct {
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
}gpioConf_t;


/*==================[internal functions declaration]=========================*/

//funcion para convertir a BCD
//bcd_number es un puntero a un arreglo donde se almacenan los n digitos
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t *bcd_number){
	for (int i = 0; i < digits; i++){ 
		*bcd_number = data % 10;
		bcd_number++; //mueve el puntero 8 bits que son los que ocupa data % 10
		data = data / 10;
	}
	return 0;
}

//funcion que cambia el estado de gpio
void cambiar_estado_gpio (uint8_t digito_BCD, gpioConf_t *conf_pines){
	for (int i = 0; i < 4; i++){
		if (digito_BCD >> i & 1){ //lo muevo a la derecha y multiplico por 1
			GPIOOn(conf_pines[i].pin);
		}
		else 
			GPIOOff(conf_pines[i].pin);
	}
}

//funcion que muestra el valor por display
void mostrar_display(uint32_t dato_display, uint8_t digitos_salida, gpioConf_t *conf_pines, gpioConf_t *conf_display){
	uint8_t arreglo_BCD[8];
	convertToBcdArray(dato_display, digitos_salida, arreglo_BCD);

	for (int i = 0; i < digitos_salida; i++){
		cambiar_estado_gpio(arreglo_BCD[i], conf_pines);
		GPIOOn(conf_display[i].pin);
		GPIOOff(conf_display[i].pin);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void){
	gpioConf_t pines[4];

	//configuracion de los pines de un display
	pines[0].pin = GPIO_20;
	pines[1].pin = GPIO_21;
	pines[2].pin = GPIO_22;
	pines[3].pin = GPIO_23;
	
	for (int i = 0; i < 4; i++){
		GPIOInit(pines[i].pin, GPIO_OUTPUT);
	}

	//configuracion de los pines para acticar cada display
	gpioConf_t pines_display[3];
	pines_display[0].pin = GPIO_19;
	pines_display[1].pin = GPIO_18;
	pines_display[2].pin = GPIO_9;

	for (int i = 0; i < 3; i++){
		GPIOInit(pines_display[i].pin, GPIO_OUTPUT);
	}
	uint32_t datos = 283;
	uint8_t digitos = 3;
	mostrar_display(datos, digitos, pines, pines_display);
	while(1);
}
/*==================[end of file]============================================*/