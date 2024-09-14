#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN PICO_DEFAULT_LED_PIN
// #define LED_PIN 18

int main()
{
    stdio_init_all();

    uint32_t ucount = 0;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        printf("Hello, world! %d\n", ucount);
        if (ucount % 2 == 0)
        {
            gpio_put(LED_PIN, 1);
        }
        else
        {
            gpio_put(LED_PIN, 0);
        }
        ucount++;

        sleep_ms(500);
    }
}
