#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "de_brujin.cpp"

#define RX_433 2 
#define TX_433 3

#define MIN_BAUDRATE_HZ     2000
#define MAX_BAUDRATE_HZ     40000
#define SKIP_BAUDRATE_HZ    500

static const char *TAG = "main";



extern "C" void app_main(void)
{
    gpio_reset_pin(RX_433);
    gpio_set_direction(RX_433, GPIO_MODE_INPUT);

    gpio_reset_pin(TX_433);
    gpio_set_direction(TX_433, GPIO_MODE_OUTPUT);


    while (1) {
    
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
