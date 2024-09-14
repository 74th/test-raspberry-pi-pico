#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_PORT spi0
#define PIN_MISO 20
#define PIN_CS 21
#define PIN_SCK 22
#define PIN_MOSI 23

int main()
{
    stdio_init_all();

    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_LSB_FIRST);
    spi_init(SPI_PORT, 250 * 1000);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_LSB_FIRST);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    uint32_t count = 0;

    while (true)
    {
        // LSB Firstがなぜか効かないので、ビットを逆転させておく
        uint8_t write_buf[10] = {0x80, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        // uint8_t write_buf[10] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint8_t read_buf[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        printf("try %d\n", count++);
        sleep_us(1);

        asm volatile("nop \n nop \n nop");
        gpio_put(PIN_CS, 0);
        asm volatile("nop \n nop \n nop");

        sleep_us(1);

        for (int i = 0; i < 5; i++)
        {
            spi_write_read_blocking(SPI_PORT, &write_buf[i], &read_buf[i], 1);
            // 1 バイトごとに時間を少し空ける必要がある
            sleep_us(10);
        }

        asm volatile("nop \n nop \n nop");
        gpio_put(PIN_CS, 1);
        asm volatile("nop \n nop \n nop");

        printf("read: 0x%x%x%x%x%x ", read_buf[0], read_buf[1], read_buf[2], read_buf[3], read_buf[4]);

        for (int i = 3; i < 5; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (read_buf[i] & 1 << j)
                {
                    printf("0");
                }
                else
                {
                    printf("1");
                }
            }
            printf(" ");
        }

        printf("\n");

        sleep_ms(500);
    }
}
