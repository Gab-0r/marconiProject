#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

class nrf24l01_driver
{
private:
public:

    nrf24l01_driver(spi_inst_t *spi); //Constructor
    ~nrf24l01_driver(); //Destructor

    void writeReg(uint8_t addr, uint8_t data); //Escribir en un registro del modulo
    void readReg(uint8_t adrr, uint8_t data); //Leer un registro del modulo
    void spiConfig(spi_inst_t *spiPort, uint16_t *cs); //Función que inicializa el spi
};