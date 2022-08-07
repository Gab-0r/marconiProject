#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SPI_PORT    spi0
#define BUF_LEN 1

#define ANGLE_MASK 0xFF
#define SERVO_MASK 0x01
#define MODE_MASK 0x02

//Modo operación (automático, control remoto)
const int opModePin = 10;
bool opMode = true;

//Para controlar el servo2
bool rightButton = false;
bool leftButton = false;
const int rightPin = 12;
const int leftPin = 15;

//Para control de servo1
const int timonPin = 28;
uint16_t potenciometro;
float grados1;

//Factor de converion del adc
const float conversion_factor = 3.3f / (1 << 12);


//EMISOR
char buffer[BUF_LEN];
uint8_t val2send = 0xf5;

void printBuff(){
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        printf("%u", buffer[i]);
    }
    printf("\n");
}

void setPeripherals();
void readVelaDegree();

int main()
{
    stdio_init_all();//Inicializar input-output
    setPeripherals();//Inicializar perifericos
    sleep_ms(5000);
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
    printf("Configurando modulo...\n");
    NRF.default_config();
    NRF.goTo_tx();
    sleep_ms(5000);

    while(1){
        
        readVelaDegree();
        val2send = grados1;
        sprintf(buffer, "%c", (char*)val2send);
        
        NRF.send(buffer);
        printf("Se envió: \n");
        printBuff();
        sleep_ms(1000);
    }
}

void setPeripherals(){
    //SYS_INIT
    adc_init();
    stdio_init_all();

    //Modo de operación
    gpio_init(opModePin);
    gpio_set_dir(opModePin, GPIO_IN);
    
    //Potenciometro
    adc_gpio_init(timonPin);
    adc_select_input(2);

    //Pulsadores
    gpio_init(rightPin);
    gpio_init(leftPin);
    gpio_set_dir(rightPin, GPIO_IN);
    gpio_set_dir(leftPin, GPIO_IN); 
}

void readVelaDegree(){
    potenciometro = adc_read();
    //printf("Raw value: 0x%03x, voltage: %f V\n", potenciometro, potenciometro * conversion_factor);
    grados1 = (potenciometro * conversion_factor) * (55);
    printf("Los grados medidos son: %f\n", grados1);
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