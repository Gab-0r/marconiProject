#include "nrf24l01.h"
#include "nrf24l01_driver.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <string.h>

#define LOW 0
#define HIGH 1

nrf24l01_driver::nrf24l01_driver(spi_inst_t *spiPort, uint16_t cs, uint16_t ce){
    spi_ = spiPort;
    cs_ = cs;
    ce_ = ce;
    gpio_init(ce_);
    gpio_set_dir(ce_, GPIO_OUT);
    gpio_put(ce_, LOW);
    spi_config();
}

nrf24l01_driver::~nrf24l01_driver(){
}

void nrf24l01_driver::spi_config(){
    spi_init(spi_, 1000000); //Inicializar SPI en el puerto spiPort a 1Mbps

    //Setear funciones de GPIO de SPI
    gpio_set_function(4, GPIO_FUNC_SPI);
    gpio_set_function(2,  GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);

    //Inicializar CS
    gpio_init(cs_);
    gpio_set_dir(cs_, GPIO_OUT);
    gpio_put(cs_, HIGH);//CS en alto para no iniciar transmisión
}

void nrf24l01_driver::default_config(){
    gpio_put(cs_, HIGH); //Apagar el SPI, por precaución
    gpio_put(ce_, LOW); //Ir a standby 1
    sleep_ms(11);

    write_reg(CONFIG, 0x0A); //Configuración registro CONFIG
    sleep_us(1500);

    write_reg(EN_AA, 0x00); //Desactivar autoACK
    write_reg(RF_CH, 120); //Establecer el canal en offset + 120MHz
    
    write_reg(RX_PW_P0, 32); //Tamaño en bytes del payload en el pipe 0
}

void nrf24l01_driver::write_reg(uint8_t addr, uint8_t data){
    uint8_t addr_ = W_REGISTER | (REGISTER_MASK & addr);
    uint8_t payload[2]; //Preparar datos para el SPI
    payload[0] = addr_;
    payload[1] = data;
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, payload, 2);//Transmisión de SPI
    gpio_put(cs_, HIGH); //Acabar con la transmisión
}

uint8_t nrf24l01_driver::read_reg(uint8_t addr){
    uint8_t readByte = 0;
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, &addr, 1);
    spi_read_blocking(spi_, NOP, &readByte, 1);
    gpio_put(cs_, HIGH);
    return readByte;
}

void nrf24l01_driver::goTo_tx(){
    uint8_t valRead = read_reg(CONFIG);
    valRead = valRead & (0 << PRIM_RX);
    write_reg(CONFIG, valRead);
    gpio_put(ce_, LOW);
    sleep_us(130);
}

void nrf24l01_driver::send(char *data){
    uint8_t cmd = W_TX_PAYLOAD;
    gpio_put(cs_, LOW);
    spi_write_blocking(spi_, &cmd, 1);
    spi_write_blocking(spi_, (uint8_t*)data, 32);
    gpio_put(cs_, HIGH);

    gpio_put(ce_, HIGH);
    sleep_us(10);
    gpio_put(ce_, LOW);
}

void nrf24l01_driver::setTX_addr(char *addr){
    uint8_t cmd = W_REGISTER | (REGISTER_MASK & TX_ADDR);
    if(strlen(addr) !=5 )return;
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, &cmd, 1);//Transmisión de SPI
    spi_write_blocking(spi_, (uint8_t*)addr, 5);//Transmisión de SPI
    gpio_put(cs_, HIGH); //Acabar con la transmisión
}

void nrf24l01_driver::goTo_rx(){
    uint8_t valRead = read_reg(CONFIG);
    valRead = valRead | (1 << PRIM_RX);
    write_reg(CONFIG, valRead);
    gpio_put(ce_, HIGH);
    sleep_us(130);
}

void nrf24l01_driver::receive(char *data){
    uint8_t cmd = R_RX_PAYLOAD;
    gpio_put(cs_, LOW);
    spi_write_blocking(spi_, &cmd,1);
    gpio_put(cs_, HIGH);
}

void nrf24l01_driver::setRX_addr(char *addr){
    uint8_t cmd = W_REGISTER | (REGISTER_MASK & RX_ADDR_P0);
    if(strlen(addr) !=5 )return;
    gpio_put(cs_, LOW); //Iniciar transmisión
    spi_write_blocking(spi_, &cmd, 1);//Transmisión de SPI
    spi_write_blocking(spi_, (uint8_t*)addr, 1);//Transmisión de SPI
    gpio_put(cs_, HIGH); //Acabar con la transmisión
}

uint8_t nrf24l01_driver::dataInc(){
    uint8_t rxstatus = read_reg(FIFO_STATUS);
    rxstatus = !(0x00000001 & rxstatus);
    return rxstatus;
}
