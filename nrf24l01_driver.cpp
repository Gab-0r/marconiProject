#include "nrf24l01.h"
#include "nrf24l01_driver.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <string.h>

#define LOW 0
#define HIGH 1

nrf24l01_driver::nrf24l01_driver(spi_inst_t *spi, uint16_t cs, uint16_t ce)
{
    spi_ = spi;
    cs_ = cs;
    ce_ = ce;
    setPins();
    spiConfig();
}

nrf24l01_driver::~nrf24l01_driver()
{
}

void nrf24l01_driver::spiConfig(){
    spi_init(spi_, 1000000 );
    gpio_set_function(2, GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);
    gpio_set_function(4, GPIO_FUNC_SPI);
}

void nrf24l01_driver::setPins(){
    gpio_init(cs_);
    gpio_init(ce_);
    gpio_set_dir(cs_, 1);
    gpio_set_dir(ce_, 1);
    gpio_put(ce_, LOW);
    gpio_put(cs_, HIGH);
}

void nrf24l01_driver::write_reg(uint8_t reg, uint8_t data){
    reg = W_REGISTER | ( REGISTER_MASK & reg);
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &reg, 1);
    spi_write_blocking(spi_, &data, 1);
    gpio_put(cs_, 1);
}

uint8_t nrf24l01_driver::read_reg(uint8_t addr){
    uint8_t valRead = 0;
    addr = ( REGISTER_MASK & addr);
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &addr,1);
    spi_read_blocking(spi_, 0xff,&valRead, 1);
    gpio_put(cs_, 1);

    return valRead;
}

void nrf24l01_driver::default_config(){
    gpio_put(cs_, 1);
    gpio_put(ce_, 0);
    sleep_ms(11);
    write_reg(CONFIG, 0x0A); // config.
    sleep_us(1500);
    write_reg(EN_AA,0); //Desactivar el autoACK
    write_reg(RF_CH, 120); //Canal
    write_reg(RX_PW_P0, 32);
}

void nrf24l01_driver::goTo_tx(){
    uint8_t valRead = read_reg(CONFIG);
    valRead = valRead & ~(1<<PRIM_RX);
    write_reg(0, valRead);
    gpio_put(ce_, 0);
    sleep_us(130);
}

void nrf24l01_driver::send(char *data){
    uint8_t cmd = W_TX_PAYLOAD;
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &cmd,1);
    spi_write_blocking(spi_, (uint8_t*)data, 32);
    gpio_put(cs_, 1);

    gpio_put(ce_, 1);
    sleep_us(10);
    gpio_put(ce_, 0);
}

void nrf24l01_driver::goTo_rx(){
    uint8_t valRead = read_reg(0);
    valRead = valRead | (1<<PRIM_RX);
    write_reg(0, valRead);
    gpio_put(ce_, 1);   
    sleep_us(130);
}

void nrf24l01_driver::receive(char *data){
    uint8_t cmd = R_RX_PAYLOAD;   
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &cmd, 1);
    spi_read_blocking(spi_, NOP, (uint8_t*)data,32);
    gpio_put(cs_, 1);;
}

uint8_t nrf24l01_driver::data_inc(){
    uint8_t rxStatus = read_reg(0x17);
    return (0x00000001 & rxStatus);
}

void nrf24l01_driver::goTo_standbyI(){
    //PENDIENTE IMPLEMENTACION
}

