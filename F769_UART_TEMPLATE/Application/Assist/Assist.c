#include "Assist.h"

uint32_t FreeRTOSRunTimeTicks;

const char *taskDebug = "taskDebug";
osThreadId taskDebugHandle;



void creatOsTaskDebug(void)
{
	osThreadDef(taskDebug, vTaskDebug, osPriorityNormal, 0, 128);
	taskDebugHandle = osThreadCreate(osThread(taskDebug),NULL);
	
}

void vTaskDebug(void const *arg)
{
    UBaseType_t   ArraySize;
    TaskStatus_t  *StatusArray;
    uint8_t       x;

    ArraySize = uxTaskGetNumberOfTasks(); //??????
    StatusArray = pvPortMalloc(ArraySize*sizeof(TaskStatus_t));
    for(;;)
    {

        if(StatusArray != NULL){ //??????

            ArraySize = uxTaskGetSystemState( (TaskStatus_t *)  StatusArray,
                                              (UBaseType_t   ) ArraySize,
                                              (uint32_t *    )  &FreeRTOSRunTimeTicks );

            printf("Name\t\tPriority\t\tNumber\t\tMinStk\t\r\n");
            for(x = 0;x<ArraySize;x++){

                printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\r\n",
                        StatusArray[x].pcTaskName,
                        (int)StatusArray[x].uxCurrentPriority,
                        (int)StatusArray[x].xTaskNumber,
                        (int)StatusArray[x].usStackHighWaterMark,
                        (int)((float)StatusArray[x].ulRunTimeCounter/FreeRTOSRunTimeTicks*100));
            }
            printf("\n\n");
        }
        vTaskDelay(2000);
    }
}

