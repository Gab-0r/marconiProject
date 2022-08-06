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
    stdio_init_all();
    sleep_ms(5000);
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
    printf("Configurando modulo...\n");
    NRF.config();
    NRF.modeRX();
    sleep_ms(5000);
    printf("Esperando datos....\n");

    while(1){
        /*
        uint8_t status = NRF.readReg(0x00);
        printf("Valor de CONFIG es: %u\n", status);
        uint8_t chanel = NRF.readReg(0x05);
        printf("Valor de RF_CH es: %u\n", chanel);
        sleep_ms(1000);
        */
       
        if(NRF.newMessage()){
            printf("Datos recibidos...\n");
            NRF.receiveMessage(buffer);
            printBuff();
            sleep_ms(1000);
        }
        
    }
}
