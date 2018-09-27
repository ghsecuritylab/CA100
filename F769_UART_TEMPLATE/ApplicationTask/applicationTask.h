#ifndef __applicationTask_H
#define __applicationTask_H

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "usart.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "lwip.h"
#include "sockets.h"
#include "api.h"

void vTaskSocket(void const *argument);
void tcpecho_thread(void *arg);
void creatOsTask(void);

#endif
