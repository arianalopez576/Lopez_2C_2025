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
#include "timer_mcu.h"
#include "analog_io_mcu.h"
#include "gpio_mcu.h"
#include "uart_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
TaskHandle_t tarea_AD = NULL;

/*==================[internal functions declaration]=========================*/
void FuncTimerA(void *param){
    vTaskNotifyGiveFromISR(tarea_AD, pdFALSE);
}

void convertir_enviar (){
	 while (true){
		uint16_t dato;
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		AnalogInputReadSingle(CH1, &dato);
	
		char arreglo_dato[30];
		sprintf(arreglo_dato, ">volts: %u,3500 \r\n", dato);
		UartSendString(UART_PC, arreglo_dato);

		//UartSendString(UART_PC,(char *) UartItoa(dato, 10));
		//UartSendString(UART_PC,"\r\n");
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

	//Inicializacion de timer
	timer_config_t mitimer = {
        TIMER_A,
        20000,
        FuncTimerA,
        NULL
    };
	TimerInit(&mitimer);
	

	xTaskCreate(&convertir_enviar, "LED_1", 4096, NULL, 5, &tarea_AD);

	//Timers
    TimerStart(mitimer.timer);

	//UART
	serial_config_t config_UART = {
		UART_PC,
		115200,
		NULL,
		NULL
	};
	UartInit(&config_UART);


}
/*==================[end of file]============================================*/