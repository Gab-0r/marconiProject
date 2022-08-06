#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

class nrf24l01_driver
{
private:
public:
    spi_inst_t *spi_;
    uint16_t cs_;
    uint16_t ce_;
    nrf24l01_driver(spi_inst_t *spiPort, uint16_t cs, uint16_t ce); //Constructor
    ~nrf24l01_driver(); //Destructor

    void spi_config(); //Función que inicializa el spi
    void write_reg(uint8_t addr, uint8_t data); //Escribir en un registro del modulo
    uint8_t read_reg(uint8_t adr); //Leer un registro del modulo
    void default_config(); //Configuración por defecto
    void goTo_tx();//Establecer el módulo en modo transmisión
    void send(char *data);//Enviar datos
    void setTX_addr(char *addr); //Dirección del TX
    void goTo_rx();//Establecer el módulo en modo recepción
    void receive(char *data);//Recibir datos
    void setRX_addr(char *addr);//Dirección del RX
    uint8_t dataInc();//Verificar si hay datos nuevos en la recepción
};