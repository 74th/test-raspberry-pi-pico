#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    uint32_t count = 0;

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(500);
        if (count % 2 == 0)
        {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
        }
        else
        {
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
        }
        count++;
    }
}
