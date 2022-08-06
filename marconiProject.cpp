#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"

#define SPI_PORT    spi0
#define BUF_LEN 32

char buffer[BUF_LEN];

void printBuff(){
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        printf("%c", buffer[i]);
    }
}

int main()
{
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    NRF.default_config();
    NRF.goTo_rx();

    while(1){
        if(NRF.dataInc()){
            printf("Datos recibidos...\n");
            NRF.receive(buffer);
            printBuff();
        }
    }
}
