#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"

#define SPI_PORT    spi0

int main()
{
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    NRF.default_config();
    NRF.goTo_tx();

    char buffer[32];

    while(1){
        sprintf(buffer, "60");
        buffer[30] = 'R';
        buffer[31] = 'O';
        NRF.send(buffer);
        sleep_ms(3000);

        sprintf(buffer, "-60");
        buffer[30] = 'R';
        buffer[31] = 'O';
        NRF.send(buffer);
        NRF.send(buffer);
        sleep_ms(3000);
    }
}
