#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"

int main()
{
    stdio_init_all();

    puts("Hello, world!");

    return 0;
}
