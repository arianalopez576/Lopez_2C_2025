/** @mainpage Proyecto 2 Ejercicio 4
 *
 * @section genDesc General Description
 *
 * El programa convierte datos analogicos a digitales, permite enviarlos por la UART y también
 * convierte digital a analogico. Estas dos funciones las realiza mediante dos tareas notificadas
 * por una funcion.
 *
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	Salida	 	| 	CHANNEL_0	|
 * |	Entrada		|	CHANNEL_1	|
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
#include "timer_mcu.h"
#include "analog_io_mcu.h"
#include "gpio_mcu.h"
#include "uart_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*==================[macros and definitions]=================================*/
#define tamano_ECG (sizeof(ECG)/sizeof(ECG[0]))
/*==================[internal data definition]===============================*/
unsigned char ECG[] = {
17,17,17,17,17,17,17,17,17,17,17,18,18,18,17,17,17,17,17,17,17,18,18,18,18,18,18,18,17,17,16,16,16,16,17,17,18,18,18,17,17,17,17,
18,18,19,21,22,24,25,26,27,28,29,31,32,33,34,34,35,37,38,37,34,29,24,19,15,14,15,16,17,17,17,16,15,14,13,13,13,13,13,13,13,12,12,
10,6,2,3,15,43,88,145,199,237,252,242,211,167,117,70,35,16,14,22,32,38,37,32,27,24,24,26,27,28,28,27,28,28,30,31,31,31,32,33,34,36,
38,39,40,41,42,43,45,47,49,51,53,55,57,60,62,65,68,71,75,79,83,87,92,97,101,106,111,116,121,125,129,133,136,138,139,140,140,139,137,
133,129,123,117,109,101,92,84,77,70,64,58,52,47,42,39,36,34,31,30,28,27,26,25,25,25,25,25,25,25,25,24,24,24,24,25,25,25,25,25,25,25,
24,24,24,24,24,24,24,24,23,23,22,22,21,21,21,20,20,20,20,20,19,19,18,18,18,19,19,19,19,18,17,17,18,18,18,18,18,18,18,18,17,17,17,17,
17,17,17
} ;

TaskHandle_t tarea_AD = NULL;
TaskHandle_t tarea_DA = NULL;

/*==================[internal functions declaration]=========================*/
/** 
 * @brief Envia notificación para la tarea analogico-digital
 * @param param Puntero a un parametro
*/
void FuncTimerA(void *param){
    vTaskNotifyGiveFromISR(tarea_AD, pdFALSE);
}

/** 
 * @brief Convierte de analogico a digital y luego envia los datos por la UART
*/
void convertir_enviar_AD (){
	 while (true){
		uint16_t dato;
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		AnalogInputReadSingle(CH1, &dato);
	
		char arreglo_dato[30];
		sprintf(arreglo_dato, ">volts: %u,3500 \r\n", dato);
		//UartSendString(UART_PC, arreglo_dato);

		UartSendString(UART_PC,(char *) UartItoa(dato, 10));
		UartSendString(UART_PC,"\r\n");
	 }
}

//otro timer otra interrupcion
/** 
 * @brief Envia notificación para la tarea digital-analogico
 * @param param Puntero a un parametro
*/
void FuncTimerB(void *param){
    vTaskNotifyGiveFromISR(tarea_DA, pdFALSE);
}

/** 
 * @brief Convierte digital a analogico
*/
void convertir_DA (){
	int i = 0;
	while(true){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		AnalogOutputWrite(ECG[i]);
		i++; 
		if (i == tamano_ECG)
		i=0;
	}   
}

/*==================[external functions definition]==========================*/
void app_main(void){
	
	analog_input_config_t miconfig_AD = {			
	CH1,
	ADC_SINGLE,
	NULL,
	NULL,
	0	
	};
	AnalogInputInit(&miconfig_AD);
    AnalogOutputInit();
	//Inicializacion de timer analogico-digital
	timer_config_t timer_AD = {
        TIMER_A,
        2000,
        FuncTimerA,
        NULL
    };
	TimerInit(&timer_AD);
	
	//Inicializacion de timer digital-analógico
	timer_config_t timer_DA = {
        TIMER_B,
        4000,
        FuncTimerB,
        NULL
    };
	TimerInit(&timer_DA);

	//Tareas conversiones
	xTaskCreate(&convertir_enviar_AD, "LED_1", 4096, NULL, 5, &tarea_AD);
	xTaskCreate(&convertir_DA, "LED_1", 4096, NULL, 5, &tarea_DA);

	//Timers
    TimerStart(timer_AD.timer);
	TimerStart(timer_DA.timer);

	//UART para conversion AD
	serial_config_t config_UART = {
		UART_PC,
		115200,
		NULL,
		NULL
	};
	UartInit(&config_UART);
}
/*==================[end of file]============================================*/