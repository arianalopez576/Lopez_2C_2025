/** @mainpage Proyecto 2 Ejercicio 1 Inciso 1
 *
 * @section genDesc General Description
 *
 * El programa permite medir la distancia a través de un sensor, y dependiendo de la misma
 * enciende los distintos LEDs. Además, el valor de la distancia se muestra en un display.
 * Las teclas permiten detener y retomar la medición o sólo detener y retomar los datos
 * mostrados en el display
 *
 * * @section hardConn Hardware Connection
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
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 24/10/2025 |   Se documenta								 |
 * | 24/10/2025 |   Se finaliza la documentacion				 |
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
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct {
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
}gpioConf_t;

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

/*==================[external functions definition]==========================*/
void app_main(void){
	LedsInit();

	//echo y trigger
	HcSr04Init(GPIO_3, GPIO_2);

	LcdItsE0803Init();
	SwitchesInit();
	while (true){
		uint16_t distancia = HcSr04ReadDistanceInCentimeters();
		//distancia representada en los leds
		actualiza_LED(distancia);

		int8_t teclas;
		teclas = SwitchesRead();

		bool activacion = true;
		bool hold = false;

		if (activacion == true){
		
			if (hold == false){
				//distancia representada en el display
				LcdItsE0803Write(distancia);
				}
			
				switch(teclas){
					case SWITCH_1:
						activacion = ! false;
					break;
					case SWITCH_2:
						hold = true;
					break;		
				}
		}
	vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
/*==================[end of file]============================================*/