#include "hardware/spi.h"
#include "hardware/gpio.h"

class nrf24l01_driver
{
private:

public:
    spi_inst_t *spi_; //Dirección del puerto SPI
    uint16_t cs_;//Chip select
    uint16_t ce_;//Chip enable

    nrf24l01_driver(spi_inst_t *port, uint16_t csn, uint16_t ce); //Constructor
    ~nrf24l01_driver();//Destructor

    void spiConfig();//Función que inicializa el SPI
    void setPins();//Inicializar pins CS y CE
    void write_reg( uint8_t reg, uint8_t data);//Escribir en registros del modulo
    uint8_t read_reg(uint8_t reg);//Leer registros del módulo
    void default_config();//Configuración del módulo

    void goTo_tx();//Ir al estado TX
    void send(char *data); //Enviar datos

    void goTo_rx();//Ir al estado RX
    void receive(char *data);//Recibir datos
    uint8_t data_inc();//Verificar si hay datos en FIFO de recepción

    void goTo_standbyI();//Ir a standby-I (Pendiente implementación)

};