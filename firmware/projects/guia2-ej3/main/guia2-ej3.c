/*! @mainpage Proyecto 2 Ejercicio 3
 *
 * @section genDesc General Description
 *
 * El programa permite leer una distancia a través del sensor, mostrar el mismo en el display
 * y dependiendo de este valor se enciende una combinación distinta de LEDs.
 * Además, el dato medido se envía por UART y a través de dos teclas se puede congelar el valor
 * mostrado en el display o detener la medición. 
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
#include "uart_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
bool activacion = true;
bool hold = false;

TaskHandle_t tarea_led_display = NULL;
TaskHandle_t tarea_teclas = NULL;

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

//Funcion para envio de la distancia por UART
/** 
 * @brief Envia el dato medido de la distancia por UART
 * @param p_dato Dato medido por el sensor
*/
void Func_Uart (uint16_t p_dato){
	char arreglo_dato[30];
	sprintf(arreglo_dato, "Distancia: %u cm\r\n", p_dato); //%u para enteros decimales sin signo (la distancia)
	UartSendString(UART_PC, arreglo_dato);
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
			actualiza_LED(distancia);
			Func_Uart(distancia);
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
 * @brief Maneja las teclas y modifica una variable cuando una de ellas es presionada.
 * @param puntero_tarea_teclas Puntero a un arreglo que contiene la tarea de las teclas
*/
void manejo_teclas(void *puntero_tarea_teclas){
	uint8_t byte_leido;
	UartReadByte(UART_PC, &byte_leido);
	char tecla = (char)byte_leido;
	if (tecla == 'O'){
		activacion =! activacion;
	}
	if (tecla =='H'){
		hold =! hold;
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	LedsInit();
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
	SwitchesInit();

	//Inicializacion de timer
	timer_config_t timer_tarea_led_display = {
        TIMER_A,
        1000000,
        FuncTimerA,
        NULL
    };
	TimerInit(&timer_tarea_led_display);
	
    TimerStart(timer_tarea_led_display.timer);

	//tarea
	xTaskCreate(&manejo_leds_display, "LED_1", 2048, NULL, 5, &tarea_led_display);
	
	//UART
	serial_config_t config_UART = {
		UART_PC,
		9600,
		manejo_teclas,
		NULL
	};
	UartInit(&config_UART);
}
/*==================[end of file]============================================*/