#include "nrf24l01.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define LOW 0
#define HIGH 1

nrf24l01_driver::nrf24l01_driver(spi_inst_t *spiPort, uint8_t *cs){
    spi_ = spi_port;
    cs_ = cs;
    spiConfig();
}

nrf24l01_driver::write_reg(uint8_t addr, uint8_t data, uint8_t bytes_ = 1){
    addr_ = W_REGISTER | (REGISTER_MASK & addr);
    uint8_t payload[2] = [addr, data]; //Preparar datos para el SPI
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, *payload, 2);//Transmisión de SPI
    gpio_put(cs, HIGH); //Acabar con la transmisión
}

nrf24l01_driver::read_reg(uint8_t *addr, uint8_t bytes_ = 1){
    uint8_t readByte;
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, *addr, 1);
    spi_read_blocking(spi_, 0, *readByte, 1);
    gpio_put(cs_, HIGH);
    return redByte;
}

nrf24l01_driver::spi_config(){
    spi_init(spi_, 1000000); //Inicializar SPI en el puerto spiPort a 1Mbps

    //Setear funciones de GPIO de SPI
    gpio_set_function(4, GPIO_FUNC_SPI);
    gpio_set_function(2,  GPIO_FUNC_SPI)
    gpio_set_function(3, GPIO_FUNC_SPI);

    //Inicializar CS
    gpio_init(cs_pin);
    gpio_setdir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, HIGH);//CS en alto para no iniciar transmisión
}

nrfl4l01::default_config(){
    gpio_put(cs, HIGH); //Apagar el SPI, por precaución
    gpio_put(ce, LOW); //Ir a standby 1
    sleep_ms(11);

    write_reg(CONFIG, 0x0A); //Configuración registro CONFIG
    sleep_us(1500);

    write_reg(EN_AA, 0x00); //Desactivar autoACK
    write_reg(RF_CH, 120); //Establecer el canal en offset + 120MHz
    
    write_reg(RX_PW_P0, 32); //Tamaño en bytes del payload en el pipe 0
}

nrf24l01::goTo_tx(){
    uint8_t valRead = read_reg(CONFIG);
    valRead = valRead & (0 << PRIM_RX);
    write_reg(CONFIG, valRead);
    sleep_us(130);
}

nrf24l01::goTo_rx(){
    uint8_t valRead = read_reg(CONFIG);
    valRead = valRead & ();
}

nrf24l01::send(char *data){
    uint8_t cmd = W_TX_PAYLOAD;
    gpio_put(cs, LOW);
    spi_write_blocking(spi_, cmd);\
    spi_write_blocking(spi_, (uint8_t*)msg, 32);
}

