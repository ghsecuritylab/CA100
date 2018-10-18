#ifndef __ASSIST_H
#define	__ASSIST_H

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "usart.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS.h"

#ifndef DEBUG_FreeRTOS
	#define DEBUG_FreeRTOS 0
#endif

void creatOsTaskDebug(void);
void vTaskDebug(void const *arg);



#endif
