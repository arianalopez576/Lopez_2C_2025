/** @mainpage Proyecto 1 Ejercicio 6
 *
 * @section genDesc General Description
 *
 * El programa permite visualizar datos (numeros de hasta 3 digitos) en una pantalla LCD.
 *
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
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 07/09/2025 | 	Creacion del documento                       |
 * | 15/09/2025 |   Se documenta								 |
 * | 15/09/2025 |   Se finaliza la documentacion				 |
 *
 * @author Ariana Lopez (lopezariana576@gmail.com)
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

/** 
 * @brief Convierte un decimal a BCD y lo almacena en un arreglo
 * @param data Dato decimal de 32 bits
 * @param digits Cantidad de digitos del decimal
 * @param bcdnumber Puntero al arreglo que almacena los datos
*/
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t *bcd_number){
	for (int i = 0; i < digits; i++){ 
		*bcd_number = data % 10;
		bcd_number++; //mueve el puntero 8 bits que son los que ocupa data % 10
		data = data / 10;
	}
	return 0;
}

/**
 * @brief Permite modificar el estado de los pines
 * @param digito_BCD Dato decimal codificado en binario
 * @param conf_pines Puntero a un arreglo que almacena la configuracion de los pines 
 */
void cambiar_estado_gpio (uint8_t digito_BCD, gpioConf_t *conf_pines){
	for (int i = 0; i < 4; i++){
		if (digito_BCD >> i & 1){ //lo muevo a la derecha y multiplico por 1
			GPIOOn(conf_pines[i].pin);
		}
		else 
			GPIOOff(conf_pines[i].pin);
	}
}

/**
 * @brief Muestra el valor por display
 * @param dato_display Dato decimal que luego sera codificado a binario
 * @param digitos_salida Cantidad de digitos del decimal
 * @param conf_pines Puntero al arreglo que almacena la conf de los pines del numero que se va a mostrar
 * @param conf_display Puntero a un arreglo que almacena la conf de los pines del digito que se va a mostrar
 */
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

	//configuracion de los pines para activar cada display
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