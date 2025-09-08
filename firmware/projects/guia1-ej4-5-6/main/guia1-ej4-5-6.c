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
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number){
	for (int i = 0; i < digits; i++){ 
		*bcd_number = data % 10;
		bcd_number++; //mueve el puntero 8 bits que son los que ocupa data % 10
		data = data / 10;
	}
	return 0;
}

//funcion que cambia el estado de gpio
void cambiar_estado_gpio (uint8_t digito_BCD, gpioConf_t *conf_pines){
	GPIOInit(conf_pines -> pin, conf_pines -> dir);
	for (int i = 0; i < 4; i++){
		if (digito_BCD >> i & 1){ //lo muevo a la derecha y multiplico por 1
			GPIOOn(conf_pines -> pin);
		}
		else 
			GPIOOff(conf_pines -> pin);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void){
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/