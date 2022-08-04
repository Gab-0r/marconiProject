#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

nrf24l01_driver::nrf24l01_driver(){
}

nrf24l01_driver::writeReg(uint8_t addr, uint8_t data){
}

nrf24l01_driver::spiConfig(spi_inst_t *spiPort, uint16_t *cs_pin){
    spi_init(spiPort, 1000000); //Inicializar SPI en el puerto spiPort a 1Mbps

    //Setear funciones de GPIO de SPI
    gpio_set_function(4, GPIO_FUNC_SPI);
    gpio_set_function(2,  GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);

    //Inicializar CS
    gpio_init(cs_pin);
    gpio_setdir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);//CS en alto para no iniciar transmisión
}