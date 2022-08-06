#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

class nrf24l01_driver
{
private:
public:
    spi_inst_t *spi_;
    uint8_t cs_;
    nrf24l01_driver(spi_inst_t *spi); //Constructor
    ~nrf24l01_driver(); //Destructor

    void write_reg(uint8_t addr, uint8_t data, uint8_t bytes_ = 1); //Escribir en un registro del modulo
    uint8_t read_reg(uint8_t adr, uint8_t bytes_ = 1); //Leer un registro del modulo
    void spi_config(spi_inst_t *spiPort, uint16_t *cs); //Función que inicializa el spi
    void default_config(); //Configuración por defecto
    void goTo_tx();//Establecer el módulo en modo transmisión
    void goTo_rx();//Establecer el módulo en modo recepción
    void send();//Enviar datos
};