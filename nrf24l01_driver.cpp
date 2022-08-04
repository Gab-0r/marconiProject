#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

nrf24l01_driver::nrf24l01_driver(spi_inst_t *spiPort, uint8_t *cs){
    spi_ = spi_port;
    cs_ = cs;
    spiConfig();
}

nrf24l01_driver::writeReg(uint8_t addr, uint8_t data){
    uint8_t payload[2] = [addr, data]; //Preparar datos para el SPI
    gpio_put(cs_, 0); //Iniciar transmisión
    spi_write_blocking(spi_, *payload, 2);//Transmisión de SPI
    gpio_put(cs, 1); //Acabar con la transmisión
}

nrf24l01_driver::readReg(uint8_t *addr, uint8_t data){
    uint8_t readByte
    gpio_put(cs_, 0); //Iniciar transmisión
    spi_write_blocking(spi_, *addr, 1);
    spi_read_blocking(spi_, 0, *readByte, 1);
    return redByte
}

nrf24l01_driver::spiConfig(){
    spi_init(spi_, 1000000); //Inicializar SPI en el puerto spiPort a 1Mbps

    //Setear funciones de GPIO de SPI
    gpio_set_function(4, GPIO_FUNC_SPI);
    gpio_set_function(2,  GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);

    //Inicializar CS
    gpio_init(cs_pin);
    gpio_setdir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);//CS en alto para no iniciar transmisión
}

