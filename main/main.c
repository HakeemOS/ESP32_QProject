#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "inttypes.h"
#include "freertos/queue.h"

#if CONFIG_FreeRTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else 
    static const BaseType_t app_cpu = 1;
#endif

static const uint8_t mQLen = 6;
static QueueHandle_t mQueue; 
static const char *sMsg = " -- Queue Demo -- "; 

void taskOne(void *arg){
    uint16_t itm; 
    char *taskName = pcTaskGetName(NULL); 
    while (1){
        if(pdTRUE == xQueueReceive(mQueue, (void *)(&itm), 0)){
            ESP_LOGI(taskName, "%" PRIu16 "\n", itm); 
        }
        vTaskDelay(50/portTICK_PERIOD_MS); 
    }

    
}

void setup(){
    vTaskDelay(1000/portTICK_PERIOD_MS); 
    char *taskName = pcTaskGetName(NULL); 
    printf("\n"); 
    printf("%s\n \n", sMsg); 
    mQueue = xQueueCreate(mQLen, sizeof(uint16_t)); 
    xTaskCreatePinnedToCore(taskOne, "Print Items", 5000, NULL, 1, NULL, app_cpu); 

}

void app_main(void){
    setup();
    static uint16_t num = 0; 
    char *taskName = pcTaskGetName(NULL); 
    vTaskDelay(2000/portTICK_PERIOD_MS);
    while(1){     
        if (pdTRUE != xQueueSend(mQueue, (void *)(&num), 10)){
            ESP_LOGW(taskName, "QUEUE FULL!"); 
        } 
        num++; 
        vTaskDelay(1000/portTICK_PERIOD_MS); 
    }
}