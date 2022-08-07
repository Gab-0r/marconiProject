#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"

#define SPI_PORT    spi0
#define BUF_LEN 1

#define ANGLE_MASK 0xFF
#define SERVO_MASK 0x01
#define MODE_MASK 0x02


//EMISOR
char buffer[BUF_LEN];
uint8_t angulo = 0xf5;

void printBuff(){
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        printf("%X", buffer[i]);
    }
    printf("\n");
}

int main()
{
    stdio_init_all();
    sleep_ms(5000);
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
    printf("Configurando modulo...\n");
    NRF.default_config();
    NRF.goTo_tx();
    sleep_ms(5000);

    while(1){
        
        //trama[0] = 180;
        //trama[1] = 0;

        sprintf(buffer, "%c", (char*)angulo);
        
        printf("Enviando datos...\n");
        NRF.send(buffer);
        printf("Se envió: \n");
        printBuff();
        sleep_ms(5000);
    }
}

//RECEPTOR
// char buffer[BUF_LEN];

// void printBuff(){
//     for (uint8_t i = 0; i < BUF_LEN; i++)
//     {
//         printf("%X", buffer[i]);
//     }
//     printf("\n");
// }

// int main()
// {
//     stdio_init_all();
//     sleep_ms(5000);
//     nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
//     printf("Configurando modulo...\n");
//     NRF.default_config();
//     NRF.goTo_rx();
//     sleep_ms(5000);
//     printf("Esperando datos....\n");

//     while(1){
//         if(!(NRF.data_inc())){
//             printf("Datos recibidos...\n");
//             NRF.receive(buffer);
//             printBuff();
//             sleep_ms(1000);
//         }
//     }
// }